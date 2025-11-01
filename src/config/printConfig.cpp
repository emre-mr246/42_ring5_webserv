/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:30 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void
printListenAddresses(const std::vector<std::pair<std::string, int> > &listenOn)
{
    std::vector<std::pair<std::string, int> >::const_iterator it;

    it = listenOn.begin();
    while (it != listenOn.end())
    {
        std::cout << "    listen " << it->first << ":" << it->second << ";" << std::endl;
        it++;
    }
}

static void printServerNames(const std::vector<std::string> &serverNames)
{
    std::vector<std::string>::const_iterator it;

    if (serverNames.empty())
        return;
    it = serverNames.begin();
    while (it != serverNames.end())
    {
        std::cout << "    server_name " << *it << ";" << std::endl;
        it++;
    }
}

static void printErrorPages(const std::map<int, std::string> &errorPages)
{
    std::map<int, std::string>::const_iterator it;

    it = errorPages.begin();
    while (it != errorPages.end())
    {
        std::cout << "    error_page " << it->first << " " << it->second << ";" << std::endl;
        it++;
    }
}

static void printLocations(const std::vector<LocationConfig> &locations)
{
    std::vector<LocationConfig>::const_iterator it;

    it = locations.begin();
    while (it != locations.end())
    {
        printSingleLocation(*it);
        it++;
    }
}

void printConfig(const Config &serverConfig)
{
    std::vector<ServerConfig>::const_iterator it;

    std::cout << std::endl;
    std::cout << "# Configuration loaded" << std::endl;
    std::cout << "# =====================" << std::endl;
    it = serverConfig.getServerConfigs().begin();
    while (it != serverConfig.getServerConfigs().end())
    {
        std::cout << std::endl
                  << "server {" << std::endl;
        printListenAddresses(it->listenOn);
        printServerNames(it->serverNames);
        if (!it->root.empty())
            std::cout << "    root " << it->root << ";" << std::endl;
        std::cout << "    client_max_body_size " << (it->clientMaxBodySize + 1023) / 1024 << "K;" << std::endl;
        printErrorPages(it->errorPages);
        printLocations(it->locations);
        std::cout << "}" << std::endl;
        it++;
    }
    std::cout << std::endl;
}
