/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSetup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 20:20:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:09 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void addUniqueAddress(std::vector<std::pair<std::string, int> > &addresses,
                      const std::string &host, int port)
{
    size_t i;

    i = 0;
    while (i < addresses.size())
    {
        if (addresses[i].first == host && addresses[i].second == port)
            return;
        i++;
    }
    addresses.push_back(std::make_pair(host, port));
}

void gatherAddresses(const Config &config,
                     std::vector<std::pair<std::string, int> > &addresses)
{
    const std::vector<ServerConfig> &servers = config.getServerConfigs();
    size_t i;
    size_t j;

    i = 0;
    while (i < servers.size())
    {
        j = 0;
        while (j < servers[i].listenOn.size())
        {
            addUniqueAddress(addresses, servers[i].listenOn[j].first,
                             servers[i].listenOn[j].second);
            j++;
        }
        i++;
    }
    if (addresses.empty())
        addUniqueAddress(addresses, "0.0.0.0", PORT);
}

int openServerSockets(const std::vector<std::pair<std::string, int> > &addresses,
                      std::vector<int> &fds)
{
    size_t i;
    int fd;

    i = 0;
    while (i < addresses.size())
    {
        fd = createListeningSocket(addresses[i].first, addresses[i].second);
        if (fd != -1)
            fds.push_back(fd);
        i++;
    }
    if (fds.empty())
    {
        std::cerr << "Error: Could not open any server sockets" << std::endl;
        return (0);
    }
    return (1);
}
