/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:14 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string extractLocationPath(const std::string &line)
{
    std::string value;
    size_t pos;
    size_t bracePos;

    pos = line.find("location");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 8));
    bracePos = value.find('{');
    if (bracePos != std::string::npos)
        value = value.substr(0, bracePos);
    value = strtrim(value);
    return (value);
}

std::string extractRoot(const std::string &line)
{
    std::string value;
    size_t pos;

    pos = line.find("root");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 4));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    return (strtrim(value));
}

std::string extractIndex(const std::string &line)
{
    std::string value;
    size_t pos;

    pos = line.find("index");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 5));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    return (strtrim(value));
}
