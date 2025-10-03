/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverExtract.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:05:30 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 02:14:25 by emgul            ###   ########.fr       */
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
