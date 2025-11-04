/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
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

static int getServerPortOrDefault(int serverFd)
{
    int port;

    port = getPortFromServerFd(serverFd);
    if (port > 0)
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

int getPortFromHostHeader(const HttpRequest &req)
{
    std::string host;
    size_t colonPos;
    std::string portStr;

    host = getHostHeaderValue(req);
    if (host.empty())
        return (getServerPortOrDefault(req.serverFd));
    colonPos = host.find(':');
    if (colonPos == std::string::npos)
        return (getServerPortOrDefault(req.serverFd));
    portStr = host.substr(colonPos + 1);
    return (parsePortString(portStr));
}

const ServerConfig *findServerByPort(const Config *config, int port)
{
    std::vector<ServerConfig>::const_iterator it;
    std::vector<std::pair<std::string, int> >::const_iterator addrIt;

    it = config->getServerConfigs().begin();
    while (it != config->getServerConfigs().end())
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
