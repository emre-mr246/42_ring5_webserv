/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:32:43 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define DEFAULT_CONFIG_PATH "config/config.rare"

struct LocationConfig
{
    std::string path;
    std::string root;
    std::string indexFile;
    bool autoindex;
    std::vector<std::string> acceptedMethods;
    std::pair<int, std::string> redirect;
    std::string uploadPath;
    size_t clientMaxBodySize;
    std::pair<std::string, std::string> cgiPass;
};

struct ServerConfig
{
    std::vector<std::pair<std::string, int> > listenOn;
    std::vector<std::string> serverNames;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;
    std::vector<LocationConfig> locations;
};

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
    Config(const Config &other);
    Config &operator=(const Config &other);
    ~Config();

    int loadConfig(const std::string &filePath);
    const std::vector<ServerConfig> &getServerConfigs() const;
};

void parserConfig(std::ifstream &configFile,
                  std::vector<ServerConfig> &serverConfigs);
void printConfig(const Config &serverConfig);
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
void parseLocationDirective(const std::string &line, LocationConfig &location);
void parseServerDirective(const std::string &line, ServerConfig &server);
int isValidLocationDirective(const std::string &line, int depth,
                             bool inLocation);
int isValidServerDirective(const std::string &line, int depth, bool inLocation);
std::string extractLocationPath(const std::string &line);
std::vector<std::string> extractAcceptedMethods(const std::string &line);
std::string extractRoot(const std::string &line);
std::string extractIndex(const std::string &line);
bool extractAutoindex(const std::string &line);
std::string extractUploadPath(const std::string &line);
std::pair<int, std::string> extractRedirect(const std::string &line);
std::pair<std::string, std::string> extractCgiPass(const std::string &line);
std::vector<std::string> extractServerNames(const std::string &line);
int validateServerConfig(const ServerConfig &server);
int validateAllServers(const std::vector<ServerConfig> &servers);
void printConfig(const Config &serverConfig);
void printAcceptedMethods(const std::vector<std::string> &acceptedMethods);
