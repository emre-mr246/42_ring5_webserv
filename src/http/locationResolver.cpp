/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationResolver.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

const LocationConfig *getLocationConfig(const HttpRequest &req,
                                        const Config *config)
{
    return (findLocation(req, config));
}

const ServerConfig *findServerForRequest(const HttpRequest &req,
                                         const Config *config)
{
    const ServerConfig *server;
    std::string host;
    int port;

    host = getHostFromRequest(req);
    port = getPortFromHostHeader(req);
    server = findServerByHost(config, host, port);
    if (!server)
        server = findServerByPort(config, port);
    return (server);
}

void resolveRoot(const LocationConfig *location, const ServerConfig *server,
                 std::string &root)
{
    if (location->root.empty() && server && !server->root.empty())
        root = server->root;
    else if (!location->root.empty())
        root = location->root;
    else
        root = "./www";
}

const LocationConfig *getDefaultLocation(const Config *config)
{
    if (!config->getServerConfigs().empty())
    {
        if (!config->getServerConfigs()[0].locations.empty())
            return (&config->getServerConfigs()[0].locations[0]);
    }
    return (NULL);
}

void getLocationSettings(const HttpRequest &req, const Config *config,
                         std::string &root, std::string &index)
{
    const LocationConfig *location;
    const ServerConfig *server;

    location = findLocation(req, config);
    if (location)
    {
        server = findServerForRequest(req, config);
        resolveRoot(location, server, root);
        index = location->indexFile;
    }
    else
    {
        root = "./www";
        index = "index.html";
    }
}
