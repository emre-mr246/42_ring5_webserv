/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostParser2.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:29 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::string stripQueryString(const std::string &uri)
{
    size_t qPos;

    qPos = uri.find('?');
    if (qPos == std::string::npos)
        return (uri);
    return (uri.substr(0, qPos));
}

int isUriMatchingLocation(const std::string &uri,
                          const std::string &locationPath)
{
    size_t i;

    if (uri == locationPath)
        return (1);
    if (locationPath == "/")
        return (1);
    if (uri.length() < locationPath.length())
        return (0);
    i = 0;
    while (i < locationPath.length())
    {
        if (uri[i] != locationPath[i])
            return (0);
        i++;
    }
    if (uri[locationPath.length()] == '/' || uri.length() == locationPath.length())
        return (1);
    return (0);
}

const LocationConfig *findBestMatchingLocation(const ServerConfig *server, const std::string &cleanUri)
{
    const LocationConfig *bestMatch;
    size_t bestMatchLen;
    size_t i;

    bestMatch = NULL;
    bestMatchLen = 0;
    i = 0;
    while (i < server->locations.size())
    {
        if (isUriMatchingLocation(cleanUri, server->locations[i].path))
        {
            if (server->locations[i].path.length() > bestMatchLen)
            {
                bestMatch = &server->locations[i];
                bestMatchLen = server->locations[i].path.length();
            }
        }
        i++;
    }
    return (bestMatch);
}

const LocationConfig *findLocation(const HttpRequest &req, const Config *config)
{
    std::string host;
    const ServerConfig *server;
    int port;
    std::string cleanUri;

    if (!config)
        return (NULL);
    host = getHostFromRequest(req);
    port = getPortFromHostHeader(req);
    if (port == -1)
        return (getDefaultLocation(config));
    server = findServerByHost(config, host, port);
    if (!server)
        server = findServerByPort(config, port);
    if (server)
    {
        cleanUri = stripQueryString(req.uri);
        return (findBestMatchingLocation(server, cleanUri));
    }
    return (getDefaultLocation(config));
}
