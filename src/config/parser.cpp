/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/09/17 14:53:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int parsePort(const std::string &s)
{
    char *endptr = NULL;
    long val = 0;

    if (s.empty())
        return (0);
    val = strtol(s.c_str(), &endptr);
    if (endptr == s.c_str() || val <= 0 || val > 65535)
        return (0);
    return (static_cast<int>(val));
}

static void addListenAddress(const std::string &value, ServerConfig &server)
{
    std::string ip;
    std::string portStr;
    size_t colonPos;
    int port;

    port = 0;
    if (value.empty())
        return;
    colonPos = value.find(':');
    if (colonPos != std::string::npos)
    {
        ip = strtrim(value.substr(0, colonPos));
        portStr = strtrim(value.substr(colonPos + 1));
        port = parsePort(portStr);
        if (port > 0)
            server.listen_on.push_back(std::make_pair(ip, port));
    }
    else
    {
        port = parsePort(value);
        if (port > 0)
            server.listen_on.push_back(std::make_pair(std::string("0.0.0.0"), port));
    }
}

int parseBodySize(const std::string &s)
{
    char *endptr = NULL;
    long val = 0;

    if (s.empty())
        return (0);
    val = strtol(s.c_str(), &endptr);
    if (endptr == s.c_str() || val < 0)
        return (0);
    return (static_cast<int>(val));
}

static void parseErrorCodes(const std::string &codes, const std::string &path, ServerConfig &server)
{
    std::string codeStr;
    int code;
    size_t start;
    size_t end;

    start = 0;
    end = 0;
    while (end != std::string::npos)
    {
        end = codes.find(' ', start);
        if (end == std::string::npos)
            codeStr = codes.substr(start);
        else
            codeStr = codes.substr(start, end - start);
        codeStr = strtrim(codeStr);
        if (!codeStr.empty())
        {
            code = parseBodySize(codeStr);
            if (code > 0)
                server.error_pages[code] = path;
        }
        start = end + 1;
    }
}

static void addErrorPages(const std::string &value, ServerConfig &server)
{
    size_t spacePos;
    std::string codes;
    std::string path;

    if (value.empty())
        return;
    spacePos = value.find_last_of(' ');
    if (spacePos == std::string::npos)
        return;
    codes = strtrim(value.substr(0, spacePos));
    path = strtrim(value.substr(spacePos + 1));
    parseErrorCodes(codes, path, server);
}

static void parseLocationDirective(const std::string &line, LocationConfig &location)
{
    std::string trimmed;

    trimmed = strtrim(line);
    if (trimmed.find("client_max_body_size") == 0)
        location.client_max_body_size = extractBodySize(line);
    else if (trimmed.find("accepted_methods") == 0)
        location.accepted_methods = extractAcceptedMethods(line);
}

static void parseServerDirective(const std::string &line, ServerConfig &server)
{
    std::string trimmed;
    std::string value;

    trimmed = strtrim(line);
    if (trimmed.find("listen") == 0)
    {
        value = extractListenValue(line);
        addListenAddress(value, server);
    }
    else if (trimmed.find("client_max_body_size") == 0)
        server.client_max_body_size = extractBodySize(line);
    else if (trimmed.find("error_page") == 0)
        addErrorPages(extractErrorPageValue(line), server);
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

static int isValidLocationDirective(const std::string &line, int depth, bool inLocation)
{
    if (depth < 1 || !inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}

static int isValidServerDirective(const std::string &line, int depth, bool inLocation)
{
    if (depth < 0 || inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}

void parserConfig(std::ifstream &configFile, std::vector<ServerConfig> &serverConfigs)
{
    int blockDepth;
    std::string line;
    ServerConfig currentServer;
    LocationConfig currentLocation;
    bool inLocation;

    blockDepth = -1;
    inLocation = false;
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
    if (blockDepth >= 0)
    {
        if (inLocation)
            currentServer.locations.push_back(currentLocation);
        serverConfigs.push_back(currentServer);
    }
}
