/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 03:47:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv)
{
    Config serverConfig;
    AddressList addresses;
    std::vector<int> serverFds;
    std::string configPath;

    initializeSignals();
    configPath = getConfigPath(argc, argv);
    if (configPath.empty())
        return (1);
    if (!serverConfig.loadConfig(configPath))
        return (1);
    gatherAddresses(serverConfig, addresses);
    if (!openServerSockets(addresses, serverFds))
        return (1);
    eventLoop(serverFds, &serverConfig);
    cleanupSockets(serverFds);
    cleanupClientBuffers();
    cleanupServerFdMap();
    cleanupResponseQueues();
    return (0);
}
