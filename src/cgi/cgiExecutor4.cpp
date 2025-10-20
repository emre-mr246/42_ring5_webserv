/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor4.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
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
    response.headers["Content-Type"] = "text/html";
    oss << output.length();
    response.headers["Content-Length"] = oss.str();
    return (response);
}

static void addContentLengthIfMissing(HttpResponse &response)
{
    std::ostringstream oss;

    if (response.headers.find("Content-Length") == response.headers.end())
    {
        oss << response.body.length();
        response.headers["Content-Length"] = oss.str();
    }
}

HttpResponse parseCgiOutput(const std::string &output)
{
    HttpResponse response;
    size_t headerEnd;
    std::string headers;

    headerEnd = output.find("\n\n");
    if (headerEnd == std::string::npos)
        return (buildSimpleResponse(output));
    response.statusCode = 200;
    response.statusMessage = "OK";
    headers = output.substr(0, headerEnd);
    response.body = output.substr(headerEnd + 2);
    parseHeadersFromCgi(headers, response);
    processStatusHeaderFromCgi(response);
    addContentLengthIfMissing(response);
    return (response);
}
