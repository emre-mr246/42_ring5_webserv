/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 02:50:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 04:36:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static size_t stringToNumber(const std::string &str)
{
    size_t result;
    size_t i;

    result = 0;
    i = 0;
    while (i < str.length())
    {
        if (str[i] >= '0' && str[i] <= '9')
            result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

static std::string extractHeaderValue(const std::string &line)
{
    size_t colonPos;

    colonPos = line.find(':');
    if (colonPos == std::string::npos)
        return ("");
    return (line.substr(colonPos + 1));
}

static std::string findHeaderLine(const std::string &headers, const std::string &headerName)
{
    size_t pos;
    size_t lineEnd;

    pos = headers.find(headerName);
    if (pos == std::string::npos)
        return ("");
    lineEnd = headers.find('\n', pos);
    if (lineEnd == std::string::npos)
        lineEnd = headers.length();
    return (headers.substr(pos, lineEnd - pos));
}

size_t parseContentLength(const std::string &headers)
{
    std::string line;
    std::string value;

    line = findHeaderLine(headers, "Content-Length:");
    if (line.empty())
        line = findHeaderLine(headers, "content-length:");
    if (line.empty())
        return (0);
    value = extractHeaderValue(line);
    return (stringToNumber(value));
}

size_t findHeaderEnd(const std::string &buffer)
{
    size_t pos;

    pos = buffer.find("\r\n\r\n");
    if (pos != std::string::npos)
        return (pos + 4);
    pos = buffer.find("\n\n");
    if (pos != std::string::npos)
        return (pos + 2);
    return (std::string::npos);
}
