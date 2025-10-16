/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static HttpResponse createSuccessResponse(const std::string &content,
                                          const std::string &mime)
{
    HttpResponse response;
    std::ostringstream oss;

    response.statusCode = 200;
    response.statusMessage = "OK";
    response.body = content;
    oss << content.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = mime;
    return (response);
}

HttpResponse handleGetRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;
    std::string content;
    std::string mimeType;
    std::string interpreter;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403));
    interpreter = getCgiInterpreter(filePath, req, config);
    if (!interpreter.empty())
        return (executeCgiScript(filePath, interpreter));
    if (!readFile(filePath, content))
        return (createErrorResponse(404));
    mimeType = getMimeType(filePath);
    return (createSuccessResponse(content, mimeType));
}
