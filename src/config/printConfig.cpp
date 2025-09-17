/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/09/17 14:53:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void printListenAddresses(const std::vector<std::pair<std::string, int> > &listen_on)
{
    std::vector<std::pair<std::string, int> >::const_iterator it;

    std::cout << "[CONFIG] Server listening on: ";
    it = listen_on.begin();
    while (it != listen_on.end())
    {
        std::cout << it->first << ":" << it->second << " ";
        it++;
    }
    std::cout << std::endl;
}

static void printErrorPages(const std::map<int, std::string> &error_pages)
{
    std::map<int, std::string>::const_iterator it;

    it = error_pages.begin();
    while (it != error_pages.end())
    {
        std::cout << "[CONFIG] Error page " << it->first << ": " << it->second << std::endl;
        it++;
    }
}

static void printAcceptedMethods(const std::vector<std::string> &accepted_methods)
{
    std::vector<std::string>::const_iterator it;

    if (accepted_methods.empty())
    {
        std::cout << "[CONFIG] Accepted methods: (none specified)" << std::endl;
        return;
    }
    std::cout << "[CONFIG] Accepted methods: ";
    it = accepted_methods.begin();
    while (it != accepted_methods.end())
    {
        std::cout << *it;
        it++;
        if (it != accepted_methods.end())
            std::cout << ", ";
    }
    std::cout << std::endl;
}

static void printLocations(const std::vector<LocationConfig> &locations)
{
    std::vector<LocationConfig>::const_iterator it;

    it = locations.begin();
    while (it != locations.end())
    {
        std::cout << "[CONFIG] Location path: " << it->path << std::endl;
        std::cout << "[CONFIG]   Client max body size: " << (it->client_max_body_size + 1023) / 1024 << " KB" << std::endl;
        printAcceptedMethods(it->accepted_methods);
        it++;
    }
}

void printConfig(const Config &serverConfig)
{
    std::vector<ServerConfig>::const_iterator it;

    it = serverConfig.getServerConfigs().begin();
    while (it != serverConfig.getServerConfigs().end())
    {
        std::cout << "[Server " << (it - serverConfig.getServerConfigs().begin() + 1) << "] ------------------------------" << std::endl;
        printListenAddresses(it->listen_on);
        std::cout << "[CONFIG] Client max body size: " << (it->client_max_body_size + 1023) / 1024 << " KB" << std::endl;
        printErrorPages(it->error_pages);
        printLocations(it->locations);
        it++;
    }
}