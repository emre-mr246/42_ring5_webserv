/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
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

void parseAndHandleRequest(const char *buf, ssize_t bytesRead,
                           int clientFd,
                           std::vector<struct pollfd> &pollFds,
                           const Config *config)
{
    HttpRequest req;
    HttpResponse response;
    std::string responseStr;

    if (!validateRequestSize(buf, bytesRead, clientFd, pollFds))
        return;
    if (!processRequestData(buf, bytesRead, req))
    {
        handleParseError(buf, bytesRead, clientFd, pollFds);
        return;
    }
    if (!checkBodySizeLimit(req, config))
    {
        response = createErrorResponse(413, req, config);
        responseStr = buildHttpResponse(response);
        setPendingResponse(clientFd, responseStr);
        updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
        return;
    }
    printHttpRequest(req);
    handleRequest(req, clientFd, pollFds, config);
}

static void sendBodySizeError(int clientFd, std::vector<struct pollfd> &pollFds,
                              const HttpRequest &tempReq, const Config *config)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(413, tempReq, config);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
}

void checkHeadersForBodySize(int clientFd, std::vector<struct pollfd> &pollFds,
                             const Config *config, int *shouldContinue)
{
    HttpRequest tempReq;
    std::string &clientBuffer = getClientBuffer(clientFd);
    size_t contentLength;
    size_t maxSize;

    *shouldContinue = 1;
    contentLength = parseContentLength(clientBuffer);
    if (contentLength == 0)
        return;
    if (contentLength > MAX_REQUEST_BUFFER)
    {
        sendErrorAndStop(clientFd, pollFds, 413, shouldContinue);
        return;
    }
    if (!processRequestData(clientBuffer.c_str(), clientBuffer.length(), tempReq))
        return;
    maxSize = getMaxBodySize(tempReq, config);
    if (contentLength > maxSize)
    {
        sendBodySizeError(clientFd, pollFds, tempReq, config);
        *shouldContinue = 0;
    }
}

int readFromClient(int clientFd, std::vector<struct pollfd> &pollFds, const Config *config)
{
    char buf[4096];
    ssize_t bytesRead;
    int wasHeadersParsed;

    bytesRead = read(clientFd, buf, sizeof(buf));
    if (bytesRead > 0)
    {
        std::string &clientBuffer = getClientBuffer(clientFd);

        updateClientTime(clientFd);
        wasHeadersParsed = areHeadersParsed(clientFd);
        clientBuffer.append(buf, bytesRead);
        return (processNewData(clientFd, pollFds, config, wasHeadersParsed));
    }
    if (bytesRead == 0)
    {
        clearClientBuffer(clientFd);
        return (0);
    }
    return (1);
}
