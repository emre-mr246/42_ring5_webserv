/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <unistd.h>

static HttpResponse createDeleteSuccessResponse(void)
{
    HttpResponse response;

    response.statusCode = 204;
    response.statusMessage = "No Content";
    response.headers["Content-Length"] = "0";
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

HttpResponse handleDeleteRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403));
    if (!deleteFileFromPath(filePath))
        return (createErrorResponse(404));
    return (createDeleteSuccessResponse());
}
