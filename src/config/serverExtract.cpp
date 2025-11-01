/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExtract.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:05:30 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:30 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string extractErrorPageValue(const std::string &line)
{
    std::string value;
    size_t pos;

    pos = line.find("error_page");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 10));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    return (strtrim(value));
}

std::string extractListenValue(const std::string &line)
{
    std::string value;
    size_t pos = 0;

    pos = line.find("listen");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 6));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    return (strtrim(value));
}

int extractBodySize(const std::string &line)
{
    std::string value;
    size_t pos;
    int bodySize;

    pos = line.find("client_max_body_size");
    if (pos == std::string::npos)
        return (1048576);
    value = strtrim(line.substr(pos + 20));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    value = strtrim(value);
    bodySize = parseBodySize(value);
    if (bodySize > 0)
        return (bodySize);
    return (1048576);
}

static void parseServerNameTokens(const std::string &value, std::vector<std::string> &names)
{
    size_t start;
    size_t end;

    start = 0;
    end = value.find(' ');
    while (end != std::string::npos)
    {
        names.push_back(value.substr(start, end - start));
        start = end + 1;
        end = value.find(' ', start);
    }
    if (start < value.length())
        names.push_back(value.substr(start));
}

std::vector<std::string> extractServerNames(const std::string &line)
{
    std::vector<std::string> names;
    std::string value;
    size_t pos;

    pos = line.find("server_name");
    if (pos == std::string::npos)
        return (names);
    value = strtrim(line.substr(pos + 11));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    value = strtrim(value);
    parseServerNameTokens(value, names);
    return (names);
}
