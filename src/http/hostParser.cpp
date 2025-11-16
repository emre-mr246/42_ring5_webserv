/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::string getHostFromRequest(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator hostIt;

    hostIt = req.headers.find("Host");
    if (hostIt == req.headers.end())
        hostIt = req.headers.find("host");
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

static std::string getHostHeaderValue(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator hostIt;

    hostIt = req.headers.find("Host");
    if (hostIt == req.headers.end())
        hostIt = req.headers.find("host");
    if (hostIt == req.headers.end())
        return ("");
    return (hostIt->second);
}

static int getPortFromSocket(int clientFd)
{
    sockaddr_in addr;
    socklen_t addrLen;

    addrLen = sizeof(addr);
    if (getsockname(clientFd, (sockaddr *)&addr, &addrLen) == -1)
        return (80);
    return (ntohs(addr.sin_port));
}

int getPortFromHostHeader(const HttpRequest &req)
{
    std::string host;
    size_t colonPos;
    std::string portStr;

    host = getHostHeaderValue(req);
    if (host.empty())
        return (getPortFromSocket(req.clientFd));
    colonPos = host.find(':');
    if (colonPos == std::string::npos)
        return (getPortFromSocket(req.clientFd));
    portStr = host.substr(colonPos + 1);
    return (parsePortString(portStr));
}

const ServerConfig *findServerByPort(const Config *config, int port)
{
    ServerConfigList::const_iterator it;
    const ServerConfigList &servers = config->getServers();
    AddressList::const_iterator addrIt;

    it = servers.begin();
    while (it != servers.end())
    {
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
