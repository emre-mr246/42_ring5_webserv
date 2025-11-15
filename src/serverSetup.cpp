/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSetup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 20:20:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int g_shutdown = 0;

static void signalHandler(int)
{
    g_shutdown = 1;
}

int getShutdownFlag(void)
{
    return (g_shutdown);
}

void initializeSignals(void)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

void addToAddressList(AddressList &addresses, const std::string &host, int port)
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

void gatherAddresses(const Config &config, AddressList &addresses)
{
    const ServerConfigList &servers = config.getServers();
    size_t i;
    size_t j;

    i = 0;
    while (i < servers.size())
    {
        j = 0;
        while (j < servers[i].listenOn.size())
        {
            addToAddressList(addresses, servers[i].listenOn[j].first,
                             servers[i].listenOn[j].second);
            j++;
        }
        i++;
    }
    if (addresses.empty())
        addToAddressList(addresses, "0.0.0.0", 8080);
}

static std::map<int, int> &getServerFdToPortMap(void)
{
    static std::map<int, int> serverFdToPort;
    return (serverFdToPort);
}

int getPortFromServerFd(int serverFd)
{
    std::map<int, int>::iterator it;

    it = getServerFdToPortMap().find(serverFd);
    if (it != getServerFdToPortMap().end())
        return (it->second);
    return (-1);
}

void cleanupServerFdMap(void)
{
    getServerFdToPortMap().clear();
}

int openServerSockets(const AddressList &addresses, std::vector<int> &fds)
{
    size_t i;
    int fd;
    std::map<int, int> &fdToPortMap = getServerFdToPortMap();

    i = 0;
    while (i < addresses.size())
    {
        fd = createListeningSocket(addresses[i].first, addresses[i].second);
        if (fd != -1)
        {
            fds.push_back(fd);
            fdToPortMap[fd] = addresses[i].second;
        }
        i++;
    }
    if (fds.empty())
    {
        std::cerr << "Error: Could not open any server sockets" << std::endl;
        return (0);
    }
    return (1);
}

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
