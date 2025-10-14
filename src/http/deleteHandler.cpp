/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:50:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/14 15:25:07 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>
#include <unistd.h>

static HttpResponse createDeleteSuccessResponse(void)
{
    HttpResponse response;
    std::string body;
    std::ostringstream oss;

    response.statusCode = 200;
    response.statusMessage = "OK";
    body = "File deleted successfully";
    response.body = body;
    oss << body.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = "text/plain";
    return (response);
}

static int deleteFileFromPath(const std::string &path)
{
    if (access(path.c_str(), F_OK) != 0)
        return (0);
    if (unlink(path.c_str()) == 0)
        return (1);
    return (0);
}

HttpResponse handleDeleteRequest(const std::string &uri)
{
    std::string filePath;

    filePath = resolveFilePath(uri);
    if (filePath.empty())
        return (createErrorResponse(403));
    if (!deleteFileFromPath(filePath))
        return (createErrorResponse(404));
    return (createDeleteSuccessResponse());
}
