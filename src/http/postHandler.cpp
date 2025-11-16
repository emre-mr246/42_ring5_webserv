/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static std::string sanitizeFileName(const std::string &name)
{
    std::string cleaned;
    size_t i;
    char c;

    cleaned = "";
    i = 0;
    while (i < name.length())
    {
        c = name[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            cleaned.push_back(c);
        else if ((c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-')
            cleaned.push_back(c);
        i++;
    }
    if (cleaned.empty())
        return ("uploadedFile");
    return (cleaned);
}

static std::string parseDispositionFilename(const std::string &disposition)
{
    size_t pos;
    size_t start;
    size_t end;

    pos = disposition.find("filename=");
    if (pos == std::string::npos)
        return ("");
    start = pos + 9;
    if (start < disposition.length() && disposition[start] == '"')
    {
        start++;
        end = disposition.find('"', start);
        if (end == std::string::npos)
            return ("");
        return (disposition.substr(start, end - start));
    }
    end = disposition.find(';', start);
    if (end == std::string::npos)
        end = disposition.length();
    return (disposition.substr(start, end - start));
}

static std::string getXFileNameHeader(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;

    it = req.headers.find("X-File-Name");
    if (it == req.headers.end())
        it = req.headers.find("x-file-name");
    if (it != req.headers.end() && !it->second.empty())
        return (it->second);
    return ("");
}

static std::string extractUploadFileName(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;
    std::string disposition;
    std::string headerName;

    it = req.headers.find("Content-Disposition");
    if (it == req.headers.end())
        it = req.headers.find("content-disposition");
    if (it != req.headers.end())
        headerName = parseDispositionFilename(it->second);
    if (headerName.empty())
        headerName = getXFileNameHeader(req);
    if (headerName.empty())
        return ("uploadedFile");
    return (sanitizeFileName(headerName));
}

static HttpResponse createPostSuccessResponse(void)
{
    HttpResponse response;
    std::string body;
    std::ostringstream oss;

    response.statusCode = 201;
    response.statusMessage = "Created";
    body = "File uploaded successfully";
    response.body = body;
    oss << body.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = "text/plain";
    return (response);
}

int handleCgiRequest(const std::string &filePath, const HttpRequest &req,
                     const Config *config, HttpResponse &response)
{
    std::string interpreter;

    interpreter = getCgiInterpreter(filePath, req, config);
    if (!interpreter.empty())
    {
        response = executeCgiScript(filePath, interpreter, req, config);
        if (response.statusCode >= 400)
        {
            response = createErrorResponse(response.statusCode);
            return (1);
        }
        return (1);
    }
    return (0);
}

static HttpResponse handlePostFileUpload(const HttpRequest &req, const Config *config)
{
    std::string uploadPath;
    std::string fileName;
    std::string filePath;
    const LocationConfig *location;
    const ServerConfig *server;
    int result;

    location = findLocation(req, config);
    server = findServerForRequest(req, config);
    resolveRoot(location, server, uploadPath);
    if (uploadPath.empty())
        return (createErrorResponse(500));
    fileName = extractUploadFileName(req);
    filePath = uploadPath + "/" + fileName;
    result = writeFile(filePath, req.body);
    if (!result)
        return (createErrorResponse(500));
    return (createPostSuccessResponse());
}

HttpResponse handlePostRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;
    HttpResponse response;

    filePath = resolveFilePath(req.uri, req, config);
    if (!filePath.empty())
    {
        if (handleCgiRequest(filePath, req, config, response))
            return (response);
    }
    return (handlePostFileUpload(req, config));
}
