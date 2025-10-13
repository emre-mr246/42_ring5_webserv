/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 03:48:01 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void
printListenAddresses(const std::vector<std::pair<std::string, int>> &listenOn)
{
    std::vector<std::pair<std::string, int>>::const_iterator it;

    std::cout << "[CONFIG] Server listening on: ";
    it = listenOn.begin();
    while (it != listenOn.end())
    {
        std::cout << it->first << ":" << it->second << " ";
        it++;
    }
    std::cout << std::endl;
}

static void printServerNames(const std::vector<std::string> &serverNames)
{
    std::vector<std::string>::const_iterator it;

    if (serverNames.empty())
        return;
    std::cout << "[CONFIG] Server names: ";
    it = serverNames.begin();
    while (it != serverNames.end())
    {
        std::cout << *it;
        it++;
        if (it != serverNames.end())
            std::cout << ", ";
    }
    std::cout << std::endl;
}

static void printErrorPages(const std::map<int, std::string> &errorPages)
{
    std::map<int, std::string>::const_iterator it;

    it = errorPages.begin();
    while (it != errorPages.end())
    {
        std::cout << "[CONFIG] Error page " << it->first << ": " << it->second
                  << std::endl;
        it++;
    }
}

static void printLocations(const std::vector<LocationConfig> &locations)
{
    std::vector<LocationConfig>::const_iterator it;

    it = locations.begin();
    while (it != locations.end())
    {
        std::cout << "[CONFIG] Location path: " << it->path << std::endl;
        std::cout << "[CONFIG]   Client max body size: "
                  << (it->clientMaxBodySize + 1023) / 1024 << " KB" << std::endl;
        printAcceptedMethods(it->acceptedMethods);
        if (!it->root.empty())
            std::cout << "[CONFIG]   Root: " << it->root << std::endl;
        if (!it->indexFile.empty())
            std::cout << "[CONFIG]   Index: " << it->indexFile << std::endl;
        std::cout << "[CONFIG]   Autoindex: " << (it->autoindex ? "on" : "off")
                  << std::endl;
        if (!it->uploadPath.empty())
            std::cout << "[CONFIG]   Upload path: " << it->uploadPath << std::endl;
        if (it->redirect.first != 0)
            std::cout << "[CONFIG]   Redirect: " << it->redirect.first << " "
                      << it->redirect.second << std::endl;
        if (!it->cgiPass.first.empty())
            std::cout << "[CONFIG]   CGI pass: " << it->cgiPass.first << " -> "
                      << it->cgiPass.second << std::endl;
        it++;
    }
}

void printConfig(const Config &serverConfig)
{
    std::vector<ServerConfig>::const_iterator it;

    it = serverConfig.getServerConfigs().begin();
    while (it != serverConfig.getServerConfigs().end())
    {
        std::cout << "[Server "
                  << (it - serverConfig.getServerConfigs().begin() + 1)
                  << "] ------------------------------" << std::endl;
        printListenAddresses(it->listenOn);
        printServerNames(it->serverNames);
        std::cout << "[CONFIG] Client max body size: "
                  << (it->clientMaxBodySize + 1023) / 1024 << " KB" << std::endl;
        printErrorPages(it->errorPages);
        printLocations(it->locations);
        it++;
        if (it != serverConfig.getServerConfigs().end())
            std::cout << std::endl;
    }
}
