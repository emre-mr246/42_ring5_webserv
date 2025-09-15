/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:07:50 by emgul            #+#    #+#              */
/*   Updated: 2025/09/15 17:05:30 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "webserv.hpp"

Config::Config() {}

Config::Config(const Config &other) : serverConfigs(other.serverConfigs) {}

Config &Config::operator=(const Config &other)
{
    if (this != &other)
    {
        serverConfigs = other.serverConfigs;
    }
    return (*this);
}

Config::~Config() {}

int readConfigFile(const std::string &filePath, std::vector<ServerConfig> &serverConfigs)
{
    std::ifstream configFile(filePath.c_str());

    if (!configFile.is_open())
    {
        std::cerr << "[ERROR] Could not open configuration file: " << filePath << std::endl;
        return (0);
    }
    std::cout << "[INFO] Reading configuration file..." << std::endl;
    parserConfig(configFile, serverConfigs);
    configFile.close();
    return (1);
}

int Config::loadConfig(const std::string &filePath)
{
    if (filePath.length() < 5 || filePath.substr(filePath.length() - 5) != ".rare")
    {
        std::cerr << "[ERROR] Configuration file must have a .rare extension." << std::endl;
        return (0);
    }
    if (readConfigFile(filePath, serverConfigs) == 0)
        return (0);
    return (1);
}

void printConfig(const Config &serverConfig)
{
    std::vector<ServerConfig>::const_iterator it;
    std::vector<std::pair<std::string, int> >::const_iterator listen_it;
    std::map<int, std::string>::const_iterator error_it;

    std::cout << "----------------------------------------" << std::endl;
    it = serverConfig.getServerConfigs().begin();
    while (it != serverConfig.getServerConfigs().end())
    {
        std::cout << "[CONFIG] Server listening on: ";
        listen_it = it->listen_on.begin();
        while (listen_it != it->listen_on.end())
        {
            std::cout << listen_it->first << ":" << listen_it->second << " ";
            listen_it++;
        }
        std::cout << std::endl;
        std::cout << "[CONFIG] Client max body size: " << it->client_max_body_size << " bytes" << std::endl;

        error_it = it->error_pages.begin();
        while (error_it != it->error_pages.end())
        {
            std::cout << "[CONFIG] Error page " << error_it->first << ": " << error_it->second << std::endl;
            error_it++;
        }
        std::cout << "----------------------------------------" << std::endl;
        it++;
    }
}

const std::vector<ServerConfig> &Config::getServerConfigs() const
{
    return (serverConfigs);
}
