/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:59 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void addMethodToList(const std::string &value, size_t &start, std::vector<std::string> &methods)
{
    size_t end;
    std::string method;

    end = value.find(' ', start);
    if (end == std::string::npos)
        end = value.length();
    method = strtrim(value.substr(start, end - start));
    if (!method.empty())
        methods.push_back(method);
    start = end + 1;
}

std::vector<std::string> extractAcceptedMethods(const std::string &line)
{
    size_t pos;
    size_t start;
    std::string value;
    std::vector<std::string> methods;

    pos = line.find("accepted_methods");
    if (pos == std::string::npos)
        return (methods);
    value = strtrim(line.substr(pos + 16));
    if (!value.empty() && value[value.length() - 1] == ';')
        value = value.substr(0, value.length() - 1);
    value = strtrim(value);
    if (value.empty())
        return (methods);
    start = 0;
    while (start < value.length())
        addMethodToList(value, start, methods);
    return (methods);
}

void addCgiPass(const std::string &line, LocationConfig &location)
{
    std::string value;
    std::string ext;
    std::string path;
    size_t pos;
    size_t spacePos;

    pos = line.find("cgi_pass");
    if (pos == std::string::npos)
        return;
    value = strtrim(line.substr(pos + 8));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    spacePos = value.find(' ');
    if (spacePos == std::string::npos)
        return;
    ext = strtrim(value.substr(0, spacePos));
    path = strtrim(value.substr(spacePos + 1));
    if (!ext.empty() && !path.empty())
        location.cgiPass[ext] = path;
}
