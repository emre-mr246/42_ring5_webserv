/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static std::string trimCarriageReturn(const std::string &str)
{
    size_t len;

    len = str.length();
    if (len > 0 && str[len - 1] == '\r')
        return (str.substr(0, len - 1));
    return (str);
}

static void extractLines(const char *data, size_t len, std::vector<std::string> &lines)
{
    std::string buffer;
    size_t i;
    size_t pos;
    std::string line;

    buffer = std::string(data, len);
    i = 0;
    while (i < buffer.length())
    {
        pos = buffer.find('\n', i);
        if (pos == std::string::npos)
            break;
        line = buffer.substr(i, pos - i);
        lines.push_back(trimCarriageReturn(line));
        i = pos + 1;
    }
}

static int isChunkedEncoding(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;

    it = req.headers.find("Transfer-Encoding");
    if (it == req.headers.end())
        it = req.headers.find("transfer-encoding");
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
    size_t headerEnd;

    buffer = std::string(data, len);
    headerEnd = findHeaderEnd(buffer);
    if (headerEnd != std::string::npos && headerEnd < len)
    {
        req.body = buffer.substr(headerEnd);
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
    if (!parseRequestLine(lines[0], req))
        return (0);
    i = 1;
    while (i < lines.size())
    {
        if (lines[i].empty())
            break;
        if (!parseHeader(lines[i], req))
            return (0);
        i++;
    }
    extractBody(data, len, req);
    return (1);
}
