/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/17 08:33:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int extractMethod(const std::string &line, HttpRequest &req)
{
    size_t pos;

    pos = line.find(' ');
    if (pos == std::string::npos)
        return (0);
    req.method = line.substr(0, pos);
    return (1);
}

static int extractUri(const std::string &line, HttpRequest &req)
{
    size_t start;
    size_t end;

    start = line.find(' ');
    if (start == std::string::npos)
        return (0);
    start++;
    end = line.find(' ', start);
    if (end == std::string::npos)
        return (0);
    req.uri = line.substr(start, end - start);
    return (1);
}

static int extractVersion(const std::string &line, HttpRequest &req)
{
    size_t pos;

    pos = line.rfind(' ');
    if (pos == std::string::npos)
        return (0);
    req.version = line.substr(pos + 1);
    return (1);
}

int parseRequestLine(const std::string &line, HttpRequest &req)
{
    if (!extractMethod(line, req))
        return (0);
    if (!extractUri(line, req))
        return (0);
    if (!extractVersion(line, req))
        return (0);
    return (1);
}
