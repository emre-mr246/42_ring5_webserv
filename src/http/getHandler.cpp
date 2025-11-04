/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
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

static int isDirectory(const std::string &path)
{
    struct stat fileInfo;

    if (stat(path.c_str(), &fileInfo) == -1)
        return (0);
    if (S_ISDIR(fileInfo.st_mode))
        return (1);
    return (0);
}

static std::string getIndexForDirectory(const std::string &dirPath, const std::string &indexFile)
{
    std::string indexPath;

    if (dirPath.empty())
        return ("");

    if (dirPath[dirPath.length() - 1] == '/')
        indexPath = dirPath + indexFile;
    else
        indexPath = dirPath + "/" + indexFile;

    return (indexPath);
}

static HttpResponse serveRegularFile(const std::string &filePath, const HttpRequest &req,
                                     const Config *config)
{
    std::string content;
    std::string mimeType;
    std::string interpreter;

    interpreter = getCgiInterpreter(filePath, req, config);
    if (!interpreter.empty())
        return (executeCgiScript(filePath, interpreter, req, config));
    if (readFile(filePath, content))
    {
        if (!isMethodAllowed(req, config))
            return (createErrorResponse(403, req, config));
        mimeType = getMimeType(filePath);
        return (createSuccessResponse(content, mimeType));
    }
    return (createErrorResponse(404, req, config));
}

static HttpResponse serveIndexFile(const std::string &filePath, const HttpRequest &req,
                                   const Config *config)
{
    std::string content;
    std::string mimeType;
    std::string indexFile;
    std::string indexPath;
    std::string interpreter;
    const LocationConfig *location;

    location = findLocation(req, config);
    if (location && !location->indexFile.empty())
        indexFile = location->indexFile;
    else
        indexFile = "index.html";
    indexPath = getIndexForDirectory(filePath, indexFile);
    if (!indexPath.empty())
    {
        interpreter = getCgiInterpreter(indexPath, req, config);
        if (!interpreter.empty())
            return (executeCgiScript(indexPath, interpreter, req, config));
        if (readFile(indexPath, content))
        {
            if (!isMethodAllowed(req, config))
                return (createErrorResponse(403, req, config));
            mimeType = getMimeType(indexPath);
            return (createSuccessResponse(content, mimeType));
        }
    }
    return (createErrorResponse(404, req, config));
}

HttpResponse handleGetRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;
    int accessResult;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403, req, config));
    accessResult = checkFileAccessible(filePath);
    if (accessResult == 0)
        return (createErrorResponse(404, req, config));
    if (accessResult == -1)
        return (createErrorResponse(403, req, config));
    if (!isDirectory(filePath))
        return (serveRegularFile(filePath, req, config));
    return (serveIndexFile(filePath, req, config));
}
