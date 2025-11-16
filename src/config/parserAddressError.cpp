/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserAddressError.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
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

static void parseCodesHelper(const std::string &codes, std::map<int, std::string> &errorPages, const std::string &path)
{
    std::string codeStr;
    int code;
    size_t start;
    size_t end;

    start = 0;
    while (start < codes.length())
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
                errorPages[code] = path;
        }
        if (end == std::string::npos)
            break;
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
    parseCodesHelper(codes, server.errorPages, path);
}

void addErrorPages(const std::string &value, LocationConfig &location)
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
    parseCodesHelper(codes, location.errorPages, path);
}
