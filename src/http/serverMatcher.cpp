/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
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
    std::vector<std::pair<std::string, int> >::const_iterator addrIt;

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
    std::vector<std::string>::const_iterator nameIt;

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
    std::vector<ServerConfig>::const_iterator serverIt;

    serverIt = config->getServerConfigs().begin();
    while (serverIt != config->getServerConfigs().end())
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
