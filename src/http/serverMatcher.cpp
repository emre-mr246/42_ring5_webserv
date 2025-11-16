/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::string stripPortFromHost(const std::string &host)
{
    size_t colonPos;

    colonPos = host.find(':');
    if (colonPos != std::string::npos)
        return (host.substr(0, colonPos));
    return (host);
}

static int serverMatchesPort(const ServerConfig &server, int port)
{
    AddressList::const_iterator addrIt;

    addrIt = server.listenOn.begin();
    while (addrIt != server.listenOn.end())
    {
        if (addrIt->second == port)
            return (1);
        addrIt++;
    }
    return (0);
}

static int serverMatchesName(const ServerConfig &server,
                             const std::string &host)
{
    StringVector::const_iterator nameIt;

    nameIt = server.serverNames.begin();
    while (nameIt != server.serverNames.end())
    {
        if (*nameIt == host)
            return (1);
        nameIt++;
    }
    return (0);
}

const ServerConfig *findServerByHost(const Config *config,
                                     const std::string &host, int port)
{
    ServerConfigList::const_iterator serverIt;
    const ServerConfigList &servers = config->getServers();

    serverIt = servers.begin();
    while (serverIt != servers.end())
    {
        if (serverMatchesPort(*serverIt, port))
        {
            if (serverMatchesName(*serverIt, host))
                return (&(*serverIt));
        }
        serverIt++;
    }
    return (NULL);
}

const ServerConfig *findFirstServer(const Config *config)
{
    const std::vector<ServerConfig> &servers = config->getServers();
    if (servers.empty())
        return (NULL);
    return (&servers[0]);
}
