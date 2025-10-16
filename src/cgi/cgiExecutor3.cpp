/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor3.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static void parseHeader(const std::string &line, HttpResponse &response)
{
    size_t colonPos;
    std::string key;
    std::string value;

    colonPos = line.find(':');
    if (colonPos == std::string::npos)
        return;
    key = line.substr(0, colonPos);
    value = line.substr(colonPos + 1);
    while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
        value = value.substr(1);
    response.headers[key] = value;
}

static void parseHeaders(const std::string &headers, HttpResponse &response)
{
    size_t pos;
    size_t start;

    start = 0;
    while (start < headers.length())
    {
        pos = headers.find('\n', start);
        if (pos == std::string::npos)
            pos = headers.length();
        if (pos > start)
            parseHeader(headers.substr(start, pos - start), response);
        start = pos + 1;
    }
}

static HttpResponse buildSimpleResponse(const std::string &output)
{
    HttpResponse response;
    std::ostringstream oss;

    response.statusCode = 200;
    response.statusMessage = "OK";
    response.body = output;
    response.headers["Content-Type"] = "text/html";
    oss << output.length();
    response.headers["Content-Length"] = oss.str();
    return (response);
}

HttpResponse parseCgiOutput(const std::string &output)
{
    HttpResponse response;
    size_t headerEnd;
    std::string headers;
    std::string body;
    std::ostringstream oss;

    headerEnd = output.find("\n\n");
    if (headerEnd == std::string::npos)
        return (buildSimpleResponse(output));
    response.statusCode = 200;
    response.statusMessage = "OK";
    headers = output.substr(0, headerEnd);
    body = output.substr(headerEnd + 2);
    response.body = body;
    parseHeaders(headers, response);
    if (response.headers.find("Content-Length") == response.headers.end())
    {
        oss << body.length();
        response.headers["Content-Length"] = oss.str();
    }
    return (response);
}
