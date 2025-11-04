/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor3.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:14 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

static void parseCgiResponseHeader(const std::string &line, HttpResponse &response)
{
    size_t colonPos;
    std::string key;
    std::string value;
    size_t endPos;

    colonPos = line.find(':');
    if (colonPos == std::string::npos)
        return;
    key = line.substr(0, colonPos);
    value = line.substr(colonPos + 1);
    while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
        value = value.substr(1);
    endPos = value.length();
    while (endPos > 0 && (value[endPos - 1] == '\r' || value[endPos - 1] == '\n' || value[endPos - 1] == ' '))
        endPos--;
    response.headers[key] = value.substr(0, endPos);
}

static int extractStatusCode(const std::string &statusValue)
{
    size_t i;
    int result;

    i = 0;
    result = 0;
    while (i < statusValue.length() && (statusValue[i] == ' ' || statusValue[i] == '\t'))
        i++;
    while (i < statusValue.length() && statusValue[i] >= '0' && statusValue[i] <= '9')
    {
        result = result * 10 + (statusValue[i] - '0');
        i++;
    }
    if (result == 0)
        return (200);
    return (result);
}

void processStatusHeaderFromCgi(HttpResponse &response)
{
    std::map<std::string, std::string>::iterator it;

    it = response.headers.find("Status");
    if (it != response.headers.end())
    {
        response.statusCode = extractStatusCode(it->second);
        response.headers.erase("Status");
    }
}

void parseHeadersFromCgi(const std::string &headers, HttpResponse &response)
{
    size_t pos;
    size_t start;

    start = 0;
    while (start < headers.length())
    {
        pos = headers.find('\n', start);
        if (pos == std::string::npos)
            pos = headers.length();
        if (pos > start)
            parseCgiResponseHeader(headers.substr(start, pos - start), response);
        start = pos + 1;
    }
}
