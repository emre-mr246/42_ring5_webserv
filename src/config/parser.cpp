/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:31 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void handleClosingBrace(configData &state, std::vector<ServerConfig> &all)
{
    if (state.depth == 1 && state.inLocation)
    {
        state.currentServer.locations.push_back(state.currentLocation);
        state.inLocation = false;
    }
    else if (state.depth == 0)
    {
        all.push_back(state.currentServer);
        state.depth = -1;
        state.inLocation = false;
    }
}

static void handleBraceDepth(const std::string &line, configData &state,
                             std::vector<ServerConfig> &all)
{
    if (line.find("{") != std::string::npos && state.depth >= 0)
        state.depth++;
    if (line.find("}") != std::string::npos && state.depth >= 0)
    {
        state.depth--;
        handleClosingBrace(state, all);
    }
}

static void initLocation(LocationConfig &loc, const std::string &line)
{
    loc = LocationConfig();
    loc.path = extractLocationPath(line);
    loc.clientMaxBodySize = 1048576;
    loc.indexFile = "index.html";
    loc.acceptedMethods.push_back("GET");
}

void updateServerBlockState(const std::string &line, configData &state,
                            std::vector<ServerConfig> &all)
{
    if (isServerBlock(line))
    {
        if (state.depth >= 0)
            all.push_back(state.currentServer);
        state.currentServer = ServerConfig();
        state.currentServer.clientMaxBodySize = 1048576;
        state.depth = 0;
        state.inLocation = false;
    }
    else if (isLocationBlock(line) && state.depth > 0)
    {
        if (state.inLocation)
            state.currentServer.locations.push_back(state.currentLocation);
        initLocation(state.currentLocation, line);
        state.inLocation = true;
    }
    handleBraceDepth(line, state, all);
}

void parserConfig(std::ifstream &configFile,
                  std::vector<ServerConfig> &serverConfigs)
{
    configData state;
    std::string line;

    initConfigState(state);
    serverConfigs.clear();
    while (std::getline(configFile, line))
    {
        line = strtrim(line);
        processConfigLine(line, state, serverConfigs);
    }
    finalizeConfigParsing(state, serverConfigs);
}
