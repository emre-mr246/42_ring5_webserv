/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 21:32:43 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

struct LocationConfig
{
    LocationConfig(): autoindex(false) {}

    std::string path;
    std::string root;
    std::string index_file;
    bool autoindex;
    std::vector<std::string> accepted_methods;
    std::pair<int, std::string> redirect;
    std::string upload_path;
    std::pair<std::string, std::string> cgi_pass;
};

struct ServerConfig
{
    ServerConfig(): client_max_body_size(1048576) {}

    std::vector<std::pair<std::string, int> > listen_on;
    std::map<int, std::string> error_pages;
    size_t client_max_body_size;
    std::vector<LocationConfig> locations;
};

class Config
{
  public:
    Config();
    ~Config();

    int loadConfig(const std::string &filePath);
    const std::vector<ServerConfig> &getServerConfigs() const;

  private:
    std::vector<ServerConfig> serverConfigs;
};