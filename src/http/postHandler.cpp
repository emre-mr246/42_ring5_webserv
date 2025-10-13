/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:12 by emgul            ###   ########.fr       */
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

static std::string extractUploadFileName(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;
    std::string disposition;
    std::string headerName;

    it = req.headers.find("Content-Disposition");
    if (it != req.headers.end())
    {
        disposition = it->second;
        headerName = parseDispositionFilename(disposition);
    }
    if (headerName.empty())
    {
        it = req.headers.find("X-File-Name");
        if (it != req.headers.end() && !it->second.empty())
            headerName = it->second;
    }
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

HttpResponse handlePostRequest(const HttpRequest &req)
{
    std::string fileName;
    std::string filePath;
    int result;

    if (req.body.empty())
        return (createErrorResponse(400));
    fileName = extractUploadFileName(req);
    filePath = "./uploads/" + fileName;
    result = writeFile(filePath, req.body);
    if (!result)
        return (createErrorResponse(500));
    return (createPostSuccessResponse());
}
