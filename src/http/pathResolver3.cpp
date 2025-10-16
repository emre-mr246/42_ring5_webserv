/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver3.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:12 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>

std::string stripPortFromHost(const std::string &host)
{
    size_t colonPos;

    colonPos = host.find(':');
    if (colonPos != std::string::npos)
        return (host.substr(0, colonPos));
    return (host);
}

int getPortFromSocket(int serverFd)
{
    struct sockaddr_in addr;
    socklen_t addrLen;

    addrLen = sizeof(addr);
    if (getsockname(serverFd, (struct sockaddr *)&addr, &addrLen) == -1)
        return (-1);
    return (ntohs(addr.sin_port));
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
