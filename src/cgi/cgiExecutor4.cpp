/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor4.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static HttpResponse buildSimpleResponse(const std::string &output)
{
    HttpResponse response;
    std::ostringstream oss;

    response.statusCode = 200;
    response.statusMessage = "OK";
    response.body = output;
    response.headers["Content-Type"] = "application/octet-stream";
    oss << output.length();
    response.headers["Content-Length"] = oss.str();
    return (response);
}

static void addContentLengthIfMissing(HttpResponse &response)
{
    std::ostringstream oss;

    oss << response.body.length();
    response.headers["Content-Length"] = oss.str();
}

HttpResponse parseCgiOutput(const std::string &output)
{
    HttpResponse response;
    size_t headerEnd;
    std::string headers;
    size_t headerEndPos;

    headerEndPos = output.find("\r\n\r\n");
    headerEnd = output.find("\n\n");
    if (headerEndPos != std::string::npos && (headerEnd == std::string::npos || headerEndPos < headerEnd))
    {
        response.statusCode = 200;
        response.statusMessage = "OK";
        headers = output.substr(0, headerEndPos);
        response.body = output.substr(headerEndPos + 4);
    }
    else if (headerEnd != std::string::npos)
    {
        response.statusCode = 200;
        response.statusMessage = "OK";
        headers = output.substr(0, headerEnd);
        response.body = output.substr(headerEnd + 2);
    }
    else
        return (buildSimpleResponse(output));
    parseHeadersFromCgi(headers, response);
    processStatusHeaderFromCgi(response);
    addContentLengthIfMissing(response);
    return (response);
}
