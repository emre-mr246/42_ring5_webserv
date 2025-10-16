/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:10:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "webserv.hpp"

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
    std::cout << "[INFO] Reading configuration file..." << std::endl;
    parserConfig(configFile, serverConfigs);
    configFile.close();
    return (1);
}
