/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printAcceptedMethods(const std::vector<std::string> &acceptedMethods)
{
    std::vector<std::string>::const_iterator it;

    if (acceptedMethods.empty())
    {
        std::cout << "[CONFIG]   Accepted methods: (none specified)" << std::endl;
        return;
    }
    std::cout << "[CONFIG]   Accepted methods: ";
    it = acceptedMethods.begin();
    while (it != acceptedMethods.end())
    {
        std::cout << *it;
        it++;
        if (it != acceptedMethods.end())
            std::cout << ", ";
    }
    std::cout << std::endl;
}

void printCgiPasses(const std::map<std::string, std::string> &cgiPass)
{
    std::map<std::string, std::string>::const_iterator it;

    it = cgiPass.begin();
    while (it != cgiPass.end())
    {
        std::cout << "[CONFIG]   CGI pass: " << it->first << " -> "
                  << it->second << std::endl;
        it++;
    }
}

void printSingleLocation(const LocationConfig &location)
{
    std::cout << "[CONFIG] Location path: " << location.path << std::endl;
    std::cout << "[CONFIG]   Client max body size: "
              << (location.clientMaxBodySize + 1023) / 1024 << " KB" << std::endl;
    printAcceptedMethods(location.acceptedMethods);
    if (!location.root.empty())
        std::cout << "[CONFIG]   Root: " << location.root << std::endl;
    if (!location.indexFile.empty())
        std::cout << "[CONFIG]   Index: " << location.indexFile << std::endl;
    std::cout << "[CONFIG]   Autoindex: " << (location.autoindex ? "on" : "off")
              << std::endl;
    if (!location.uploadPath.empty())
        std::cout << "[CONFIG]   Upload path: " << location.uploadPath << std::endl;
    if (location.redirect.first != 0)
        std::cout << "[CONFIG]   Redirect: " << location.redirect.first << " "
                  << location.redirect.second << std::endl;
    if (!location.cgiPass.empty())
        printCgiPasses(location.cgiPass);
}
