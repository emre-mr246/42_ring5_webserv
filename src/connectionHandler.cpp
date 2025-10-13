/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:13 by emgul            ###   ########.fr       */
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
    if (setNonblocking(clientFd) == -1)
    {
        close(clientFd);
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

static void handleRequest(const HttpRequest &req, int clientFd,
                          std::vector<struct pollfd> &pollFds)
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
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}

static void parseAndHandleRequest(const char *buf, ssize_t bytesRead,
                                  int clientFd,
                                  std::vector<struct pollfd> &pollFds)
{
    HttpRequest req;
    HttpResponse response;
    std::string responseStr;

    if (!processRequestData(buf, bytesRead, req))
    {
        response = createErrorResponse(413);
        responseStr = buildHttpResponse(response);
        setPendingResponse(clientFd, responseStr);
        updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
        return;
    }
    printHttpRequest(req);
    handleRequest(req, clientFd, pollFds);
}

int readFromClient(int clientFd, std::vector<struct pollfd> &pollFds)
{
    char buf[4096];
    ssize_t bytesRead;

    bytesRead = read(clientFd, buf, sizeof(buf));
    if (bytesRead > 0)
    {
        updateClientTime(clientFd);
        parseAndHandleRequest(buf, bytesRead, clientFd, pollFds);
        return (1);
    }
    if (bytesRead == 0)
        return (0);
    return (1);
}
