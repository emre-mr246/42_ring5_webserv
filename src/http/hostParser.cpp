/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::string getHostFromRequest(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator hostIt;

    hostIt = req.headers.find("Host");
    if (hostIt != req.headers.end())
        return (stripPortFromHost(hostIt->second));
    return ("");
}

int parsePortString(const std::string &portStr)
{
    int port;
    size_t i;

    port = 0;
    i = 0;
    while (i < portStr.length() && portStr[i] >= '0' && portStr[i] <= '9')
    {
        port = port * 10 + (portStr[i] - '0');
        i++;
    }
    if (port > 0 && port <= 65535)
        return (port);
    return (80);
}

int getPortFromHostHeader(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator hostIt;
    std::string host;
    size_t colonPos;
    std::string portStr;

    hostIt = req.headers.find("Host");
    if (hostIt == req.headers.end())
        return (80);
    host = hostIt->second;
    colonPos = host.find(':');
    if (colonPos == std::string::npos)
        return (80);
    portStr = host.substr(colonPos + 1);
    return (parsePortString(portStr));
}

const ServerConfig *findServerByPort(const Config *config, int port)
{
    std::vector<ServerConfig>::const_iterator it;

    it = config->getServerConfigs().begin();
    while (it != config->getServerConfigs().end())
    {
        std::vector<std::pair<std::string, int> >::const_iterator addrIt;
        addrIt = it->listenOn.begin();
        while (addrIt != it->listenOn.end())
        {
            if (addrIt->second == port)
                return (&(*it));
            addrIt++;
        }
        it++;
    }
    return (NULL);
}

const LocationConfig *findLocation(const HttpRequest &req,
                                   const Config *config)
{
    std::string host;
    const ServerConfig *server;
    int port;

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
        if (!server->locations.empty())
            return (&server->locations[0]);
        return (NULL);
    }
    return (getDefaultLocation(config));
}
