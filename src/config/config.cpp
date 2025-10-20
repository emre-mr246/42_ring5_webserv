/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:07:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "webserv.hpp"

Config::Config() {}

Config::~Config() {}

int Config::loadConfig(const std::string &filePath)
{
    if (filePath.length() < 5 ||
        filePath.substr(filePath.length() - 5) != ".rare")
    {
        std::cerr << "[ERROR] Configuration file must have a .rare extension."
                  << std::endl;
        return (0);
    }
    if (readConfigFile(filePath, serverConfigs) == 0)
        return (0);
    if (validateAllServers(serverConfigs))
    {
        std::cerr << "[ERROR] Configuration validation failed." << std::endl;
        return (0);
    }
    return (1);
}

const std::vector<ServerConfig> &Config::getServerConfigs() const
{
    return (serverConfigs);
}
