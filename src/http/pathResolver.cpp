/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int hasDangerousPattern(const std::string &uri)
{
    size_t i;

    i = 0;
    while (i < uri.length())
    {
        if (uri[i] == '.' && uri[i + 1] == '.')
            return (1);
        i++;
    }
    return (0);
}

static int isPathSafe(const std::string &uri)
{
    if (hasDangerousPattern(uri))
        return (0);
    if (uri.find("//") != std::string::npos)
        return (0);
    return (1);
}

static std::string buildPath(const std::string &uri, const std::string &root,
                             const std::string &index)
{
    std::string path;

    if (uri == "/")
    {
        if (!index.empty())
            path = root + "/" + index;
        else
            path = root + "/index.html";
    }
    else
        path = root + uri;
    return (path);
}

std::string resolveFilePath(const std::string &uri, const HttpRequest &req,
                            const Config *config)
{
    std::string root;
    std::string index;

    if (!isPathSafe(uri))
        return ("");
    getLocationSettings(req, config, root, index);
    return (buildPath(uri, root, index));
}
