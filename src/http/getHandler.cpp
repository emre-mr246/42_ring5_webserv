/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

static HttpResponse createSuccessResponse(const std::string &content, const std::string &mime)
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

static int checkFileAccessible(const std::string &path)
{
    struct stat fileInfo;

    if (stat(path.c_str(), &fileInfo) == -1)
        return (0);
    if (access(path.c_str(), R_OK) == -1)
        return (-1);
    return (1);
}

HttpResponse handleGetRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;
    std::string content;
    std::string mimeType;
    std::string interpreter;
    int accessResult;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403, req, config));
    interpreter = getCgiInterpreter(filePath, req, config);
    if (!interpreter.empty())
        return (executeCgiScript(filePath, interpreter, req, config));
    accessResult = checkFileAccessible(filePath);
    if (accessResult == 0)
        return (createErrorResponse(404, req, config));
    if (accessResult == -1)
        return (createErrorResponse(403, req, config));
    if (!readFile(filePath, content))
        return (createErrorResponse(404, req, config));
    if (!isMethodAllowed(req, config))
        return (createErrorResponse(403, req, config));
    mimeType = getMimeType(filePath);
    return (createSuccessResponse(content, mimeType));
}
