/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:31 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static HttpResponse createDeleteSuccessResponse(void)
{
    HttpResponse response;

    response.statusCode = 204;
    response.statusMessage = "No Content";
    response.headers["Content-Length"] = "0";
    return (response);
}

static int checkFileExists(const std::string &path)
{
    if (access(path.c_str(), F_OK) != 0)
        return (0);
    return (1);
}

HttpResponse handleDeleteRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403));
    if (!checkFileExists(filePath))
        return (createErrorResponse(404));
    return (createDeleteSuccessResponse());
}
