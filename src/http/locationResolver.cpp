/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationResolver.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

const ServerConfig *findServerForRequest(const HttpRequest &req, const Config *config)
{
    const ServerConfig *server;
    std::string host;
    int port;

    host = getHostFromRequest(req);
    port = getPortFromHostHeader(req);
    server = findServerByHost(config, host, port);
    if (!server)
        server = findServerByPort(config, port);
    if (!server)
        server = findFirstServer(config);
    return (server);
}

void resolveRoot(const LocationConfig *location, const ServerConfig *server,
                 std::string &root)
{
    if (location && !location->root.empty())
        root = location->root;
    else if (server && !server->root.empty())
        root = server->root;
    else
        root = "";
}

const LocationConfig *getDefaultLocation(const Config *config)
{
    if (!config->getServers().empty())
    {
        if (!config->getServers()[0].locations.empty())
            return (&config->getServers()[0].locations[0]);
    }
    return (NULL);
}

void getLocationSettings(const HttpRequest &req, const Config *config, std::string &root, std::string &index)
{
    const LocationConfig *location;
    const ServerConfig *server;

    location = findLocation(req, config);
    server = findServerForRequest(req, config);
    resolveRoot(location, server, root);
    if (location && !location->indexFile.empty())
        index = location->indexFile;
    else
        index = "";
}

int isMethodAllowed(const HttpRequest &req, const Config *config)
{
    const LocationConfig *location;
    size_t i;
    std::string methodName;

    location = findLocation(req, config);
    if (!location)
        return (1);
    if (location->acceptedMethods.empty())
        return (1);
    methodName = req.method;
    i = 0;
    while (i < location->acceptedMethods.size())
    {
        if (location->acceptedMethods[i] == methodName)
            return (1);
        i++;
    }
    return (0);
}
