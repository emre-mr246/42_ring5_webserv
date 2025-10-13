/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 03:48:01 by emgul            ###   ########.fr       */
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

HttpResponse handleGetRequest(const std::string &uri)
{
    std::string filePath;
    std::string content;
    std::string mimeType;

    filePath = resolveFilePath(uri);
    if (filePath.empty())
        return (createErrorResponse(403));
    if (!readFile(filePath, content))
        return (createErrorResponse(404));
    mimeType = getMimeType(filePath);
    return (createSuccessResponse(content, mimeType));
}
