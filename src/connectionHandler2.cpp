/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler2.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:09 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int handleClientData(std::vector<struct pollfd> &pollFds, size_t i, const Config *config)
{
    int clientFd;

    clientFd = pollFds[i].fd;
    if (hasPendingResponse(clientFd) && shouldCloseConnection(clientFd))
        return (1);
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
    bool shouldClose;

    clientFd = pollFds[i].fd;
    if (!hasPendingResponse(clientFd))
    {
        updateClientEvents(pollFds, clientFd, POLLIN);
        return (1);
    }
    updateClientTime(clientFd);
    shouldClose = shouldCloseConnection(clientFd);
    status = sendPendingResponse(clientFd);
    if (status == 0)
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    if (status == 2)
    {
        if (shouldClose)
        {
            close(clientFd);
            clearPendingResponse(clientFd);
            removeClientFromPoll(pollFds, i);
            return (0);
        }
        updateClientEvents(pollFds, clientFd, POLLIN);
    }
    return (1);
}

void sendErrorAndStop(int clientFd, std::vector<struct pollfd> &pollFds,
                      int statusCode, int *shouldContinue)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(statusCode);
    response.headers["Connection"] = "close";
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr, true);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
    *shouldContinue = 0;
}
