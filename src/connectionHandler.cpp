/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
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

    while (1)
    {
        clientFd = acceptClientConnection(serverFd);
        if (clientFd == -1)
            break;
        addClientToPoll(pollFds, clientFd);
        setClientServerFd(clientFd, serverFd);
    }
}

void parseAndHandleRequest(const char *buf, ssize_t bytesRead,
                           int clientFd,
                           std::vector<struct pollfd> &pollFds,
                           const Config *config)
{
    HttpRequest req;
    HttpResponse response;
    std::string responseStr;

    if (!processRequestData(buf, bytesRead, req))
    {
        handleParseError(buf, bytesRead, clientFd, pollFds);
        return;
    }
    req.serverFd = getClientServerFd(clientFd);
    if (!checkBodySizeLimit(req, config))
    {
        response = createErrorResponse(413, req, config);
        responseStr = buildHttpResponse(response);
        setPendingResponse(clientFd, responseStr, true);
        updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
        return;
    }
    handleRequest(req, clientFd, pollFds, config);
}

static int handleClientDataHelper(int clientFd, std::vector<struct pollfd> &pollFds,
                                  const Config *config, const char *buf, ssize_t bytesRead,
                                  int wasHeadersParsed)
{
    std::string &clientBuffer = getClientBuffer(clientFd);
    size_t contentLength;

    updateClientTime(clientFd);
    clientBuffer.append(buf, bytesRead);
    if (!wasHeadersParsed && areHeadersParsed(clientFd))
    {
        contentLength = parseContentLength(clientBuffer);
        if (contentLength > 0 && contentLength < MAX_REQUEST_BUFFER)
            clientBuffer.reserve(contentLength + 8192);
    }
    return (processNewData(clientFd, pollFds, config, wasHeadersParsed));
}

static int handleClientDisconnectHelper(int clientFd, std::vector<struct pollfd> &pollFds,
                                        const Config *config)
{
    std::string &clientBuffer = getClientBuffer(clientFd);

    if (hasPendingResponse(clientFd))
        return (1);
    if (isRequestComplete(clientFd) && clientBuffer.length() > 0 &&
        clientBuffer.length() <= MAX_REQUEST_BUFFER)
    {
        parseAndHandleRequest(clientBuffer.c_str(), clientBuffer.length(),
                              clientFd, pollFds, config);
        clearClientBuffer(clientFd);
        return (1);
    }
    clearClientBuffer(clientFd);
    return (0);
}

int readFromClient(int clientFd, std::vector<struct pollfd> &pollFds, const Config *config)
{
    char buf[FILE_READ_BUFFER_SIZE];
    ssize_t bytesRead;
    int wasHeadersParsed;

    bytesRead = read(clientFd, buf, sizeof(buf));
    if (bytesRead > 0)
    {
        wasHeadersParsed = areHeadersParsed(clientFd);
        return (handleClientDataHelper(clientFd, pollFds, config, buf, bytesRead, wasHeadersParsed));
    }
    if (bytesRead == 0)
        return (handleClientDisconnectHelper(clientFd, pollFds, config));
    clearClientBuffer(clientFd);
    return (0);
}
