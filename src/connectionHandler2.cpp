/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler2.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:09 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int handleClientData(std::vector<pollfd> &pollFds, size_t i, const Config *config)
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

static int handleSendComplete(int clientFd, std::vector<pollfd> &pollFds, size_t i)
{
    close(clientFd);
    clearPendingResponse(clientFd);
    removeClientFromPoll(pollFds, i);
    return (0);
}

static int handleSendPartial(int clientFd, std::vector<pollfd> &pollFds, int shouldClose)
{
    if (shouldClose)
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        return (0);
    }
    updateClientEvents(pollFds, clientFd, POLLIN);
    return (1);
}

int handleClientWrite(std::vector<pollfd> &pollFds, size_t i)
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
        return (handleSendComplete(clientFd, pollFds, i));
    if (status == 2)
        return (handleSendPartial(clientFd, pollFds, shouldCloseConnection(clientFd)));
    return (1);
}

void sendErrorAndStop(int clientFd, std::vector<pollfd> &pollFds,
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
