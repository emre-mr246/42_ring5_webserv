/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationExtract.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/09/17 14:53:08 by emgul            ###   ########.fr       */
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

std::vector<std::string> extractAcceptedMethods(const std::string &line)
{
    size_t pos;
    size_t start;
    size_t end;
    std::string value;
    std::string method;
    std::vector<std::string> methods;

    pos = line.find("accepted_methods");
    if (pos == std::string::npos)
        return (methods);
    value = strtrim(line.substr(pos + 16));
    if (!value.empty() && value[value.length() - 1] == ';')
        value = value.substr(0, value.length() - 1);
    start = 0;
    while (start < value.length())
    {
        end = value.find(' ', start);
        if (end == std::string::npos)
            end = value.length();
        method = strtrim(value.substr(start, end - start));
        if (!method.empty())
            methods.push_back(method);
        start = end + 1;
    }
    return (methods);
}