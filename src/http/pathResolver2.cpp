/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver2.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static std::string getHostFromRequest(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator hostIt;

    hostIt = req.headers.find("Host");
    if (hostIt != req.headers.end())
        return (stripPortFromHost(hostIt->second));
    return ("");
}

static const LocationConfig *getDefaultLocation(const Config *config)
{
    if (!config->getServerConfigs().empty())
    {
        if (!config->getServerConfigs()[0].locations.empty())
            return (&config->getServerConfigs()[0].locations[0]);
    }
    return (NULL);
}

static const LocationConfig *findLocation(const HttpRequest &req,
                                          const Config *config)
{
    std::string host;
    const ServerConfig *server;
    int port;

    if (!config)
        return (NULL);
    host = getHostFromRequest(req);
    port = getPortFromSocket(req.serverFd);
    if (port == -1)
        return (getDefaultLocation(config));
    server = findServerByHost(config, host, port);
    if (server)
    {
        if (!server->locations.empty())
            return (&server->locations[0]);
        return (NULL);
    }
    return (getDefaultLocation(config));
}

const LocationConfig *getLocationConfig(const HttpRequest &req,
                                        const Config *config)
{
    return (findLocation(req, config));
}

void getLocationSettings(const HttpRequest &req, const Config *config,
                         std::string &root, std::string &index)
{
    const LocationConfig *location;

    location = findLocation(req, config);
    if (location)
    {
        root = location->root;
        index = location->indexFile;
    }
    else
    {
        root = "./www";
        index = "index.html";
    }
}
