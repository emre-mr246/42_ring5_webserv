/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 02:14:25 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void handleBraceDepth(const std::string &line, int &depth, ServerConfig &current, std::vector<ServerConfig> &all, LocationConfig &currentLocation, bool &inLocation)
{
    if (line.find("{") != std::string::npos && depth >= 0)
        depth++;
    if (line.find("}") != std::string::npos && depth >= 0)
    {
        depth--;
        if (depth == 1 && inLocation)
        {
            current.locations.push_back(currentLocation);
            inLocation = false;
        }
        else if (depth == 0)
        {
            all.push_back(current);
            depth = -1;
            inLocation = false;
        }
    }
}

static void updateServerBlockState(const std::string &line, int &depth, ServerConfig &current, std::vector<ServerConfig> &all, LocationConfig &currentLocation, bool &inLocation)
{
    if (isServerBlock(line))
    {
        if (depth >= 0)
            all.push_back(current);
        current = ServerConfig();
        depth = 0;
        inLocation = false;
    }
    else if (isLocationBlock(line) && depth > 0)
    {
        if (inLocation)
            current.locations.push_back(currentLocation);
        currentLocation = LocationConfig();
        currentLocation.path = extractLocationPath(line);
        inLocation = true;
    }
    handleBraceDepth(line, depth, current, all, currentLocation, inLocation);
}

static void finalizeConfig(int depth, bool inLocation, ServerConfig &current, LocationConfig &currentLocation, std::vector<ServerConfig> &all)
{
    if (depth >= 0)
    {
        if (inLocation)
            current.locations.push_back(currentLocation);
        all.push_back(current);
    }
}

void parserConfig(std::ifstream &configFile, std::vector<ServerConfig> &serverConfigs)
{
    int blockDepth = -1;
    bool inLocation = false;
    std::string line;
    ServerConfig currentServer;
    LocationConfig currentLocation;

    serverConfigs.clear();
    while (std::getline(configFile, line))
    {
        line = strtrim(line);
        if (line.empty() || isComment(line))
            continue;
        updateServerBlockState(line, blockDepth, currentServer, serverConfigs, currentLocation, inLocation);
        if (isValidServerDirective(line, blockDepth, inLocation))
            parseServerDirective(line, currentServer);
        else if (isValidLocationDirective(line, blockDepth, inLocation))
            parseLocationDirective(line, currentLocation);
    }
    finalizeConfig(blockDepth, inLocation, currentServer, currentLocation, serverConfigs);
}
