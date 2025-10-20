/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler3.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/21 01:53:36 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int validateRequestSize(const char *buf, ssize_t bytesRead, int clientFd, std::vector<struct pollfd> &pollFds)
{
    std::string buffer;
    size_t contentLength;
    HttpResponse response;
    std::string responseStr;

    buffer.assign(buf, bytesRead);
    contentLength = parseContentLength(buffer);
    if (contentLength > MAX_REQUEST_BUFFER)
    {
        response = createErrorResponse(413);
        responseStr = buildHttpResponse(response);
        setPendingResponse(clientFd, responseStr);
        updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
        return (0);
    }
    return (1);
}

void handleParseError(const char *buf, ssize_t bytesRead, int clientFd, std::vector<struct pollfd> &pollFds)
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
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}

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
    return (1048576);
}

int checkBodySizeLimit(const HttpRequest &req, const Config *config)
{
    std::map<std::string, std::string>::const_iterator headerIt;
    size_t contentLength;
    size_t maxSize;
    size_t i;

    headerIt = req.headers.find("Content-Length");
    if (headerIt == req.headers.end())
        headerIt = req.headers.find("content-length");
    contentLength = 0;
    if (headerIt != req.headers.end())
    {
        i = 0;
        while (i < headerIt->second.length())
        {
            if (headerIt->second[i] >= '0' && headerIt->second[i] <= '9')
                contentLength = contentLength * 10 + (headerIt->second[i] - '0');
            i++;
        }
    }
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
                   std::vector<struct pollfd> &pollFds, const Config *config)
{
    HttpResponse response;
    std::string responseStr;

    if (!validateHttpRequest(req))
        response = createErrorResponse(400);
    else if (!isMethodAllowed(req, config))
        response = createErrorResponse(405, req, config);
    else if (req.method == "GET")
        response = handleGetRequest(req, config);
    else if (req.method == "POST")
        response = handlePostRequest(req, config);
    else if (req.method == "DELETE")
        response = handleDeleteRequest(req, config);
    else
        response = createErrorResponse(405);
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
}
