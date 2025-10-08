/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 14:45:40 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int acceptClientConnection(int serverFd)
{
    int clientFd;

    clientFd = accept(serverFd, NULL, NULL);
    if (clientFd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (-1);
        printError("accept()");
        return (-1);
    }
    return (clientFd);
}

void handleNewConnection(std::vector<struct pollfd> &pollFds, int serverFd)
{
    int clientFd;

    clientFd = acceptClientConnection(serverFd);
    if (clientFd == -1)
        return;
    addClientToPoll(pollFds, clientFd);
}

static void handleRequest(const HttpRequest &req, int clientFd)
{
    HttpResponse response;
    std::string responseStr;

    if (!validateHttpRequest(req))
        response = createErrorResponse(400);
    else if (req.method == "GET")
        response = handleGetRequest(req.uri);
    else if (req.method == "POST")
        response = handlePostRequest(req);
    else
        response = createErrorResponse(405);
    responseStr = buildHttpResponse(response);
    sendResponseToClient(clientFd, responseStr);
}

static void parseAndHandleRequest(const char *buf, ssize_t bytesRead, int clientFd)
{
    HttpRequest req;
    HttpResponse response;
    std::string responseStr;

    if (!processRequestData(buf, bytesRead, req))
    {
        response = createErrorResponse(413);
        responseStr = buildHttpResponse(response);
        sendResponseToClient(clientFd, responseStr);
        return;
    }
    printHttpRequest(req);
    handleRequest(req, clientFd);
}

int readFromClient(int clientFd)
{
    char buf[4096];
    ssize_t bytesRead;

    bytesRead = read(clientFd, buf, sizeof(buf));
    if (bytesRead > 0)
    {
        parseAndHandleRequest(buf, bytesRead, clientFd);
        return (1);
    }
    if (bytesRead == 0)
        return (0);
    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        return (1);
    printError("read()");
    return (0);
}

int handleClientData(std::vector<struct pollfd> &pollFds, size_t i)
{
    int clientFd;

    clientFd = pollFds[i].fd;
    if (!readFromClient(clientFd))
    {
        close(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    return (1);
}
