/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printAcceptedMethods(const std::vector<std::string> &acceptedMethods)
{
    std::vector<std::string>::const_iterator it;

    if (acceptedMethods.empty())
        return;
    std::cout << "        allow ";
    it = acceptedMethods.begin();
    while (it != acceptedMethods.end())
    {
        std::cout << *it;
        it++;
        if (it != acceptedMethods.end())
            std::cout << " ";
    }
    std::cout << ";" << std::endl;
}

void printCgiPasses(const std::map<std::string, std::string> &cgiPass)
{
    std::map<std::string, std::string>::const_iterator it;

    it = cgiPass.begin();
    while (it != cgiPass.end())
    {
        std::cout << "        cgi_pass " << it->first << " " << it->second << ";" << std::endl;
        it++;
    }
}

void printSingleLocation(const LocationConfig &location)
{
    std::cout << "    location " << location.path << " {" << std::endl;
    std::cout << "        client_max_body_size " << (location.clientMaxBodySize + 1023) / 1024 << "K;" << std::endl;
    printAcceptedMethods(location.acceptedMethods);
    if (!location.root.empty())
        std::cout << "        root " << location.root << ";" << std::endl;
    if (!location.indexFile.empty())
        std::cout << "        index " << location.indexFile << ";" << std::endl;
    if (!location.cgiPass.empty())
        printCgiPasses(location.cgiPass);
    std::cout << "    }" << std::endl;
}
