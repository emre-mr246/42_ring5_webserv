/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/05 13:39:50 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static size_t findHeaderEnd(const std::string &buffer)
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

static void extractLines(const char *data, size_t len, std::vector<std::string> &lines)
{
    std::string buffer;
    size_t i;
    size_t pos;

    buffer = std::string(data, len);
    i = 0;
    while (i < buffer.length())
    {
        pos = buffer.find('\n', i);
        if (pos == std::string::npos)
            break;
        lines.push_back(buffer.substr(i, pos - i));
        i = pos + 1;
    }
}

static int isChunkedEncoding(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;

    it = req.headers.find("Transfer-Encoding");
    if (it != req.headers.end())
    {
        if (it->second.find("chunked") != std::string::npos)
            return (1);
    }
    return (0);
}

static void extractBody(const char *data, size_t len, HttpRequest &req)
{
    std::string buffer;
    size_t header_end;

    buffer = std::string(data, len);
    header_end = findHeaderEnd(buffer);
    if (header_end != std::string::npos && header_end < len)
    {
        req.body = buffer.substr(header_end);
        if (isChunkedEncoding(req))
            req.body = decodeChunkedBody(req.body);
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
    extractBody(data, len, req);
    return (1);
}
