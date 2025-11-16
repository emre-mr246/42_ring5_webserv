/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:32:43 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:01 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define DEFAULT_CONFIG_PATH "config/config.rare"

typedef std::vector<std::string> StringVector;
typedef std::pair<std::string, int> AddressPair;
typedef std::vector<std::pair<std::string, int> > AddressList;

struct LocationConfig
{
    std::string path;
    std::string root;
    std::string indexFile;
    StringVector acceptedMethods;
    size_t clientMaxBodySize;
    std::map<std::string, std::string> cgiPass;
    std::map<int, std::string> errorPages;
    bool autoindex;
    std::string redirectUrl;
    int redirectCode;
};

struct ServerConfig
{
    AddressList listenOn;
    StringVector serverNames;
    std::string root;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;
    std::vector<LocationConfig> locations;
};

typedef std::vector<ServerConfig> ServerConfigList;

struct configData
{
    int depth;
    bool inLocation;
    ServerConfig currentServer;
    LocationConfig currentLocation;
};

class Config
{
  private:
    std::vector<ServerConfig> serverConfigs;

  public:
    Config();
    ~Config();

    int loadConfig(const std::string &filePath);
    const std::vector<ServerConfig> &getServers() const;
};

int readConfigFile(const std::string &filePath, std::vector<ServerConfig> &serverConfigs);
void parserConfig(std::ifstream &configFile, std::vector<ServerConfig> &serverConfigs);
void initConfigState(configData &state);
void updateServerBlockState(const std::string &line, configData &state, std::vector<ServerConfig> &all);
void processConfigLine(const std::string &line, configData &state, std::vector<ServerConfig> &serverConfigs);
void finalizeConfigParsing(configData &state, std::vector<ServerConfig> &serverConfigs);
int isComment(const std::string &line);
bool isServerBlock(const std::string &line);
bool isLocationBlock(const std::string &line);
std::string extractErrorPageValue(const std::string &line);
std::string extractListenValue(const std::string &line);
int extractBodySize(const std::string &line);
int parseBodySize(const std::string &s);
int parsePort(const std::string &s);
void addListenAddress(const std::string &value, ServerConfig &server);
void addErrorPages(const std::string &value, ServerConfig &server);
void addErrorPages(const std::string &value, LocationConfig &location);
void parseLocationDirective(const std::string &line, LocationConfig &location);
void parseServerDirective(const std::string &line, ServerConfig &server);
int isValidLocationDirective(const std::string &line, int depth, bool inLocation);
int isValidServerDirective(const std::string &line, int depth, bool inLocation);
std::string extractLocationPath(const std::string &line);
std::vector<std::string> extractAcceptedMethods(const std::string &line);
std::string extractRoot(const std::string &line);
std::string extractIndex(const std::string &line);
bool extractAutoindex(const std::string &line);
void addCgiPass(const std::string &line, LocationConfig &location);
std::vector<std::string> extractServerNames(const std::string &line);
int validateServerConfig(const ServerConfig &server);
int validateAllServers(const std::vector<ServerConfig> &servers);
void parseRedirectDirective(const std::string &line, LocationConfig &location);
