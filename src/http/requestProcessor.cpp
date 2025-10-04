/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 21:29:35 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static void extractLines(const char *data, size_t len, std::vector<std::string> &lines)
{
    std::string buffer;
    size_t i;

    buffer = std::string(data, len);
    i = 0;
    while (i < buffer.length())
    {
        size_t pos = buffer.find('\n', i);
        if (pos == std::string::npos)
            break;
        lines.push_back(buffer.substr(i, pos - i));
        i = pos + 1;
    }
}

int processRequestData(const char *data, size_t len, HttpRequest &req)
{
    std::vector<std::string> lines;
    size_t i;

    extractLines(data, len, lines);
    if (lines.empty())
        return (0);
    parseRequestLine(lines[0], req);
    i = 1;
    while (i < lines.size())
    {
        parseHeader(lines[i], req);
        i++;
    }
    return (1);
}
