/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:21:54 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int validatePort(int port)
{
    if (port < 1 || port > 65535)
    {
        std::cerr << "Error: Port must be between 1-65535\n";
        return (1);
    }
    return (0);
}

static int checkDuplicateServer(const ServerConfig &s1, const ServerConfig &s2)
{
    size_t i;
    size_t j;

    i = 0;
    while (i < s1.listenOn.size())
    {
        j = 0;
        while (j < s2.listenOn.size())
        {
            if (s1.listenOn[i] == s2.listenOn[j])
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

int validateServerConfig(const ServerConfig &server)
{
    size_t i;

    if (server.listenOn.empty())
    {
        std::cerr << "Error: listen directive is required\n";
        return (1);
    }
    i = 0;
    while (i < server.listenOn.size())
    {
        if (validatePort(server.listenOn[i].second))
            return (1);
        i++;
    }
    return (0);
}

int validateAllServers(const std::vector<ServerConfig> &servers)
{
    size_t i;
    size_t j;

    i = 0;
    while (i < servers.size())
    {
        if (validateServerConfig(servers[i]))
            return (1);
        j = i + 1;
        while (j < servers.size())
        {
            if (checkDuplicateServer(servers[i], servers[j]))
            {
                std::cerr << "Error: Duplicate server block detected\n";
                return (1);
            }
            j++;
        }
        i++;
    }
    return (0);
}
