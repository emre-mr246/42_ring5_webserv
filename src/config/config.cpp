/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:07:50 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:30 by emgul            ###   ########.fr       */
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

int readConfigFile(const std::string &filePath,
                   std::vector<ServerConfig> &serverConfigs)
{
    std::ifstream configFile(filePath.c_str());

    if (!configFile.is_open())
    {
        std::cerr << "[ERROR] Could not open configuration file: " << filePath
                  << std::endl;
        return (0);
    }
    if (DEBUG_MODE)
        std::cout << "[INFO] Reading configuration file..." << std::endl;
    parserConfig(configFile, serverConfigs);
    configFile.close();
    return (1);
}
