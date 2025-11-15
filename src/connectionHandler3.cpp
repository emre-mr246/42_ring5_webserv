/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler3.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

void handleParseError(const char *buf, ssize_t bytesRead, int clientFd, std::vector<pollfd> &pollFds)
{
    HttpResponse response;
    std::string responseStr;
    size_t contentLength;

    if ((size_t)bytesRead > MAX_REQUEST_BUFFER)
    {
        response = createErrorResponse(413);
        responseStr = buildHttpResponse(response);
        setPendingResponse(clientFd, responseStr);
        updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
        return;
    }
    contentLength = parseContentLength(std::string(buf, bytesRead));
    if (contentLength > MAX_REQUEST_BUFFER)
        response = createErrorResponse(413);
    else
        response = createErrorResponse(400);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr, true);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}

int checkBodySizeLimit(const HttpRequest &req, const Config *config)
{
    std::map<std::string, std::string>::const_iterator headerIt;
    size_t contentLength;
    size_t maxSize;

    headerIt = req.headers.find("Content-Length");
    if (headerIt == req.headers.end())
        headerIt = req.headers.find("content-length");
    if (headerIt != req.headers.end())
        contentLength = parseContentLengthValue(headerIt->second);
    else
        contentLength = req.body.length();
    if (contentLength == 0)
        return (1);
    maxSize = getMaxBodySize(req, config);
    if (contentLength > maxSize)
        return (0);
    return (1);
}

void handleRequest(const HttpRequest &req, int clientFd,
                   std::vector<pollfd> &pollFds, const Config *config)
{
    HttpResponse response;
    std::string responseStr;
    bool shouldClose;

    response = routeRequest(req, config);
    shouldClose = shouldCloseAfterResponse(req);
    if (shouldClose)
        response.headers["Connection"] = "close";
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr, shouldClose);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}
