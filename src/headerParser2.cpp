/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headerParser2.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 14:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int extractHeaderName(const std::string &line, std::string &name)
{
    size_t pos;

    pos = line.find(':');
    if (pos == std::string::npos)
        return (0);
    name = line.substr(0, pos);
    return (1);
}

static std::string extractHeaderValueForRequest(const std::string &line)
{
    size_t pos;
    std::string value;

    pos = line.find(':');
    if (pos == std::string::npos)
        return ("");
    value = line.substr(pos + 1);
    return (strtrim(value));
}

static int isHeaderSizeValid(const std::string &name, const std::string &value)
{
    size_t maxNameLen;
    size_t maxValueLen;

    maxNameLen = 256;
    maxValueLen = 8192;
    if (name.length() > maxNameLen)
        return (0);
    if (value.length() > maxValueLen)
        return (0);
    return (1);
}

int parseHeader(const std::string &line, HttpRequest &req)
{
    std::string name;
    std::string value;

    if (line.empty())
        return (1);
    if (!extractHeaderName(line, name))
        return (0);
    value = extractHeaderValueForRequest(line);
    if (!isHeaderSizeValid(name, value))
        return (0);
    req.headers[name] = value;
    return (1);
}
