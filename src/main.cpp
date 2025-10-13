/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 03:47:21 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initializeSignals(void) { signal(SIGPIPE, SIG_IGN); }

std::string getConfigPath(int argc, char **argv)
{
    std::string configPath;

    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
        return ("");
    }
    if (argc == 2)
        configPath = argv[1];
    else
        configPath = DEFAULT_CONFIG_PATH;
    return (configPath);
}

void cleanupSockets(const std::vector<int> &serverFds)
{
    size_t i;

    i = 0;
    while (i < serverFds.size())
    {
        close(serverFds[i]);
        i++;
    }
}

int main(int argc, char **argv)
{
    Config serverConfig;
    std::vector<std::pair<std::string, int> > addresses;
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
    eventLoop(serverFds);
    cleanupSockets(serverFds);
    return (0);
}
