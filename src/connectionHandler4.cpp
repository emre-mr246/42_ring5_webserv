/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler4.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:22 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

size_t parseContentLengthValue(const std::string &value)
{
    size_t result;
    size_t i;

    result = 0;
    i = 0;
    while (i < value.length())
    {
        if (value[i] >= '0' && value[i] <= '9')
            result = result * 10 + (value[i] - '0');
        i++;
    }
    return (result);
}

static HttpResponse handleHeadResponse(const HttpResponse &getResponse)
{
    HttpResponse response;
    std::ostringstream lengthStream;
    size_t bodySize;

    response = getResponse;
    bodySize = response.body.length();
    response.body = "";
    lengthStream << bodySize;
    response.headers["Content-Length"] = lengthStream.str();
    return (response);
}

static HttpResponse handleMethodNotAllowed(const HttpRequest &req, const Config *config)
{
    HttpResponse response;
    std::ostringstream lengthStream;
    size_t bodySize;

    response = createErrorResponse(405, req, config);
    if (req.method == "HEAD")
    {
        bodySize = response.body.length();
        response.body = "";
        if (response.headers.find("Content-Length") == response.headers.end())
        {
            lengthStream << bodySize;
            response.headers["Content-Length"] = lengthStream.str();
        }
    }
    return (response);
}

HttpResponse routeRequest(const HttpRequest &req, const Config *config)
{
    HttpResponse response;
    HttpRequest headAsGet;

    if (!validateHttpRequest(req))
        return (createErrorResponse(400));
    if (!isMethodAllowed(req, config))
        return (handleMethodNotAllowed(req, config));
    if (req.method == "HEAD")
    {
        headAsGet = req;
        headAsGet.method = "GET";
        response = handleGetRequest(headAsGet, config);
        return (handleHeadResponse(response));
    }
    if (req.method == "GET")
        return (handleGetRequest(req, config));
    if (req.method == "POST")
        return (handlePostRequest(req, config));
    if (req.method == "DELETE")
        return (handleDeleteRequest(req, config));
    if (req.method == "PUT")
        return (handlePutRequest(req, config));
    return (createErrorResponse(405));
}

bool shouldCloseAfterResponse(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;
    bool shouldClose;

    shouldClose = false;
    it = req.headers.find("Connection");
    if (it == req.headers.end())
        it = req.headers.find("connection");
    if (it != req.headers.end() && it->second == "close")
        shouldClose = true;
    return (shouldClose);
}
