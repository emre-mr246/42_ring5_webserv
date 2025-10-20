/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler2.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:09 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int handleClientData(std::vector<struct pollfd> &pollFds, size_t i, const Config *config)
{
    int clientFd;

    clientFd = pollFds[i].fd;
    if (!readFromClient(clientFd, pollFds, config))
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    return (1);
}

int handleClientWrite(std::vector<struct pollfd> &pollFds, size_t i)
{
    int clientFd;
    int status;

    clientFd = pollFds[i].fd;
    if (!hasPendingResponse(clientFd))
    {
        updateClientEvents(pollFds, clientFd, POLLIN);
        return (1);
    }
    status = sendPendingResponse(clientFd);
    if (status == 0)
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    if (status == 2)
        updateClientEvents(pollFds, clientFd, POLLIN);
    return (1);
}

void sendErrorAndStop(int clientFd, std::vector<struct pollfd> &pollFds,
                      int statusCode, int *shouldContinue)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(statusCode);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
    *shouldContinue = 0;
}

static int handleBufferOverflow(int clientFd, std::vector<struct pollfd> &pollFds)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(413);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
    return (1);
}

int processNewData(int clientFd, std::vector<struct pollfd> &pollFds, const Config *config, int wasHeadersParsed)
{
    std::string &clientBuffer = getClientBuffer(clientFd);
    int shouldContinue;

    if (clientBuffer.length() > MAX_REQUEST_BUFFER)
        return (handleBufferOverflow(clientFd, pollFds));
    if (!wasHeadersParsed && areHeadersParsed(clientFd))
    {
        checkHeadersForBodySize(clientFd, pollFds, config, &shouldContinue);
        if (!shouldContinue)
            return (1);
    }
    if (isRequestComplete(clientFd))
    {
        if (clientBuffer.length() > MAX_REQUEST_BUFFER)
            return (handleBufferOverflow(clientFd, pollFds));
        parseAndHandleRequest(clientBuffer.c_str(), clientBuffer.length(),
                              clientFd, pollFds, config);
        clearClientBuffer(clientFd);
    }
    return (1);
}
