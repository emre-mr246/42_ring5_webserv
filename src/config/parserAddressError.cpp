/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserAddressError.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void addListenAddress(const std::string &value, ServerConfig &server)
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
            server.listenOn.push_back(std::make_pair(ip, port));
    }
    else
    {
        port = parsePort(value);
        if (port > 0)
            server.listenOn.push_back(std::make_pair(std::string("0.0.0.0"), port));
    }
}

static void parseErrorCodes(const std::string &codes, const std::string &path,
                            ServerConfig &server)
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
                server.errorPages[code] = path;
        }
        start = end + 1;
    }
}

void addErrorPages(const std::string &value, ServerConfig &server)
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
