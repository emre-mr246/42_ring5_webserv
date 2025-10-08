/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 12:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static HttpResponse createSuccessResponse(const std::string &content, const std::string &mime)
{
    HttpResponse response;
    std::ostringstream oss;

    response.status_code = 200;
    response.status_message = "OK";
    response.body = content;
    oss << content.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = mime;
    return (response);
}

HttpResponse handleGetRequest(const std::string &uri)
{
    std::string file_path;
    std::string content;
    std::string mime_type;

    file_path = resolveFilePath(uri);
    if (file_path.empty())
        return (createErrorResponse(403));
    if (!readFile(file_path, content))
        return (createErrorResponse(404));
    mime_type = getMimeType(file_path);
    return (createSuccessResponse(content, mime_type));
}
