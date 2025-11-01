/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionValidator.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

size_t getMaxBodySize(const HttpRequest &req, const Config *config)
{
    const LocationConfig *location;
    const ServerConfig *server;

    location = findLocation(req, config);
    if (location && location->clientMaxBodySize > 0)
        return (location->clientMaxBodySize);
    server = findServerForRequest(req, config);
    if (server && server->clientMaxBodySize > 0)
        return (server->clientMaxBodySize);
    return (DEFAULT_BODY_SIZE);
}

static int detectChunkedEncoding(const std::string &clientBuffer)
{
    if (clientBuffer.find("Transfer-Encoding: chunked") != std::string::npos ||
        clientBuffer.find("transfer-encoding: chunked") != std::string::npos)
        return (1);
    return (0);
}

static int detectCgiInterpreter(const HttpRequest &req, const Config *config)
{
    std::string interpreter;

    interpreter = getCgiInterpreter(req.uri, req, config);
    return (!interpreter.empty());
}

static void sendErrorResponse(int clientFd, std::vector<struct pollfd> &pollFds,
                              int statusCode, const HttpRequest &req,
                              const Config *config, int shouldClose)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(statusCode, req, config);
    if (shouldClose)
        response.headers["Connection"] = "close";
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr, shouldClose);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
}

static void sendBodySizeError(int clientFd, std::vector<struct pollfd> &pollFds,
                              const HttpRequest &tempReq, const Config *config)
{
    sendErrorResponse(clientFd, pollFds, 413, tempReq, config, 1);
}

static int validateBodySize(int clientFd, std::vector<struct pollfd> &pollFds,
                            const HttpRequest &req, const Config *config,
                            size_t contentLength, int isCgi, int *shouldContinue)
{
    size_t maxSize;

    if (!isCgi && contentLength > MAX_REQUEST_BUFFER)
    {
        sendErrorAndStop(clientFd, pollFds, 413, shouldContinue);
        return (0);
    }
    maxSize = getMaxBodySize(req, config);
    if (contentLength > 0 && contentLength > maxSize)
    {
        sendBodySizeError(clientFd, pollFds, req, config);
        *shouldContinue = 0;
        return (0);
    }
    return (1);
}

static int validateChunkedSize(int clientFd, std::vector<struct pollfd> &pollFds,
                               const HttpRequest &req, const Config *config,
                               const std::string &clientBuffer, int *shouldContinue)
{
    size_t headerEnd;
    size_t maxSize;

    headerEnd = findHeaderEnd(clientBuffer);
    maxSize = getMaxBodySize(req, config);
    if (headerEnd != std::string::npos && clientBuffer.length() - headerEnd > maxSize)
    {
        sendBodySizeError(clientFd, pollFds, req, config);
        *shouldContinue = 0;
        return (0);
    }
    return (1);
}

static void sendContinueIfExpected(int clientFd, const HttpRequest &req)
{
    std::string continueResponse;
    std::map<std::string, std::string>::const_iterator it;

    it = req.headers.find("Expect");
    if (it == req.headers.end())
        it = req.headers.find("expect");
    if (it != req.headers.end() && it->second == "100-continue")
    {
        continueResponse = "HTTP/1.1 100 Continue\r\n\r\n";
        write(clientFd, continueResponse.c_str(), continueResponse.length());
    }
}

void checkHeadersForBodySize(int clientFd, std::vector<struct pollfd> &pollFds,
                             const Config *config, int *shouldContinue)
{
    HttpRequest tempReq;
    std::string &clientBuffer = getClientBuffer(clientFd);
    size_t contentLength;
    int isCgi;
    int isChunked;

    *shouldContinue = 1;
    contentLength = parseContentLength(clientBuffer);
    isChunked = detectChunkedEncoding(clientBuffer);
    if (!processRequestData(clientBuffer.c_str(), clientBuffer.length(), tempReq))
        return;
    tempReq.serverFd = getClientServerFd(clientFd);
    if (contentLength == 0 && !isChunked)
        return;
    if (contentLength == 0)
        return;
    isCgi = detectCgiInterpreter(tempReq, config);
    if (!validateBodySize(clientFd, pollFds, tempReq, config, contentLength, isCgi, shouldContinue))
        return;
    if (isChunked && !validateChunkedSize(clientFd, pollFds, tempReq, config, clientBuffer, shouldContinue))
        return;
    sendContinueIfExpected(clientFd, tempReq);
}
