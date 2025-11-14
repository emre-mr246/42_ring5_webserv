/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:31 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initConfigState(configData &state)
{
    state.depth = -1;
    state.inLocation = false;
    state.currentServer.clientMaxBodySize = 1048576;
    state.currentLocation.clientMaxBodySize = 1048576;
    state.currentLocation.indexFile = "index.html";
    state.currentLocation.acceptedMethods.push_back("GET");
}

void processConfigLine(const std::string &line, configData &state, std::vector<ServerConfig> &serverConfigs)
{
    if (line.empty() || isComment(line))
        return;
    updateServerBlockState(line, state, serverConfigs);
    if (isValidServerDirective(line, state.depth, state.inLocation))
        parseServerDirective(line, state.currentServer);
    else if (isValidLocationDirective(line, state.depth, state.inLocation))
        parseLocationDirective(line, state.currentLocation);
}

void finalizeConfigParsing(configData &state, std::vector<ServerConfig> &serverConfigs)
{
    if (state.depth >= 0)
    {
        if (state.inLocation)
            state.currentServer.locations.push_back(state.currentLocation);
        serverConfigs.push_back(state.currentServer);
    }
}
