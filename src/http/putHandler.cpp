/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <fstream>
#include <libgen.h>
#include <sys/stat.h>

static int createDirectoryPath(const std::string &path)
{
    std::string dir;
    size_t pos;

    pos = 0;
    while ((pos = path.find('/', pos)) != std::string::npos)
    {
        dir = path.substr(0, pos);
        if (!dir.empty() && mkdir(dir.c_str(), 0755) != 0)
        {
            if (errno != EEXIST)
                return (0);
        }
        pos++;
    }
    return (1);
}

static int writeFileContent(const std::string &path, const std::string &content)
{
    std::ofstream file;

    if (!createDirectoryPath(path))
        return (0);
    file.open(path.c_str());
    if (!file.is_open())
        return (0);
    file.write(content.c_str(), content.length());
    file.close();
    return (1);
}

HttpResponse handlePutRequest(const HttpRequest &req, const Config *config)
{
    std::string filePath;
    HttpResponse response;

    filePath = resolveFilePath(req.uri, req, config);
    if (filePath.empty())
        return (createErrorResponse(403));
    if (!writeFileContent(filePath, req.body))
        return (createErrorResponse(403));
    response.statusCode = 201;
    response.statusMessage = "Created";
    response.headers["Content-Length"] = "0";
    return (response);
}
