/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/21 01:53:36 by emgul            ###   ########.fr       */
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

static std::string stripLocationPrefix(const std::string &uri, const std::string &locationPath)
{
    if (locationPath.empty() || locationPath == "/")
        return (uri);
    if (uri == locationPath)
        return (std::string("/"));
    if (uri.find(locationPath) != 0)
        return (uri);
    if (uri.length() == locationPath.length())
        return (std::string("/"));
    return (uri.substr(locationPath.length()));
}

static std::string buildPath(const std::string &uri, const std::string &root, const std::string &index)
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

std::string resolveFilePath(const std::string &uri, const HttpRequest &req, const Config *config)
{
    std::string root;
    std::string index;
    std::string cleanUri;
    std::string relativeUri;
    const LocationConfig *location;

    cleanUri = uri;
    if (cleanUri.find('?') != std::string::npos)
        cleanUri = cleanUri.substr(0, cleanUri.find('?'));
    if (!isPathSafe(cleanUri))
        return ("");
    getLocationSettings(req, config, root, index);
    location = findLocation(req, config);
    if (location)
        relativeUri = stripLocationPrefix(cleanUri, location->path);
    else
        relativeUri = cleanUri;
    return (buildPath(relativeUri, root, index));
}
