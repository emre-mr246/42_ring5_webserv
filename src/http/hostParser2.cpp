/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostParser2.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
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

static int isMethodAllowedForLocation(const LocationConfig &location, const std::string &method)
{
    size_t j;

    if (location.acceptedMethods.empty())
        return (1);
    j = 0;
    while (j < location.acceptedMethods.size())
    {
        if (location.acceptedMethods[j] == method)
            return (1);
        j++;
    }
    return (0);
}

static void updateMatchesIfBetter(const LocationConfig &location, size_t locationLen,
                                  const LocationConfig *&bestMatch, size_t &bestMatchLen,
                                  const LocationConfig *&fallbackMatch, size_t &fallbackMatchLen,
                                  int methodAllowed)
{
    if (methodAllowed)
    {
        if (locationLen > bestMatchLen)
        {
            bestMatch = &location;
            bestMatchLen = locationLen;
        }
    }
    else if (!fallbackMatch || locationLen > fallbackMatchLen)
    {
        fallbackMatch = &location;
        fallbackMatchLen = locationLen;
    }
}

const LocationConfig *findBestMatchingLocation(const ServerConfig *server, const std::string &cleanUri, const std::string &method)
{
    const LocationConfig *bestMatch;
    const LocationConfig *fallbackMatch;
    size_t bestMatchLen;
    size_t fallbackMatchLen;
    size_t i;
    int methodAllowed;

    bestMatch = NULL;
    fallbackMatch = NULL;
    bestMatchLen = 0;
    fallbackMatchLen = 0;
    i = 0;
    while (i < server->locations.size())
    {
        if (isUriMatchingLocation(cleanUri, server->locations[i].path))
        {
            if (server->locations[i].path.length() > bestMatchLen)
            {
                methodAllowed = isMethodAllowedForLocation(server->locations[i], method);
                updateMatchesIfBetter(server->locations[i], server->locations[i].path.length(),
                                      bestMatch, bestMatchLen,
                                      fallbackMatch, fallbackMatchLen,
                                      methodAllowed);
            }
        }
        i++;
    }
    if (bestMatch)
        return (bestMatch);
    return (fallbackMatch);
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
        return (findBestMatchingLocation(server, cleanUri, req.method));
    }
    return (getDefaultLocation(config));
}
