/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:09 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int acceptClientConnection(int serverFd)
{
    int clientFd;

    clientFd = accept(serverFd, NULL, NULL);
    if (clientFd == -1)
        return (-1);
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
                          std::vector<struct pollfd> &pollFds,
                          const Config *config)
{
    HttpResponse response;
    std::string responseStr;

    if (!validateHttpRequest(req))
        response = createErrorResponse(400);
    else if (req.method == "GET")
        response = handleGetRequest(req, config);
    else if (req.method == "POST")
        response = handlePostRequest(req);
    else if (req.method == "DELETE")
        response = handleDeleteRequest(req, config);
    else
        response = createErrorResponse(405);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}

static void parseAndHandleRequest(const char *buf, ssize_t bytesRead,
                                  int clientFd,
                                  std::vector<struct pollfd> &pollFds,
                                  const Config *config)
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
    handleRequest(req, clientFd, pollFds, config);
}

int readFromClient(int clientFd, std::vector<struct pollfd> &pollFds,
                   const Config *config)
{
    char buf[4096];
    ssize_t bytesRead;

    bytesRead = read(clientFd, buf, sizeof(buf));
    if (bytesRead > 0)
    {
        updateClientTime(clientFd);
        parseAndHandleRequest(buf, bytesRead, clientFd, pollFds, config);
        return (1);
    }
    if (bytesRead == 0)
        return (0);
    return (1);
}
