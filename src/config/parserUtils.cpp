/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:05:30 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int isComment(const std::string &line)
{
    size_t i;

    i = 0;
    while (i < line.length())
    {
        if (!isspace(line[i]))
        {
            if (line[i] == '#')
                return (1);
            else
                return (0);
        }
        i++;
    }
    return (0);
}

bool isServerBlock(const std::string &line)
{
    std::string trimmed;

    trimmed = strtrim(line);
    return (trimmed == "server" || trimmed == "server {");
}

bool isLocationBlock(const std::string &line)
{
    std::string trimmed;

    trimmed = strtrim(line);
    return (trimmed.find("location") == 0);
}

int parsePort(const std::string &s)
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

int parseBodySize(const std::string &s)
{
    char *endptr = NULL;
    long val = 0;
    long multiplier = 1;

    if (s.empty())
        return (0);
    val = strtol(s.c_str(), &endptr);
    if (endptr == s.c_str() || val < 0)
        return (0);
    if (*endptr == 'k' || *endptr == 'K')
        multiplier = 1024;
    else if (*endptr == 'm' || *endptr == 'M')
        multiplier = 1024 * 1024;
    else if (*endptr == 'g' || *endptr == 'G')
        multiplier = 1024 * 1024 * 1024;
    return (static_cast<int>(val * multiplier));
}
