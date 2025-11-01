/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedDecoder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:29 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static size_t parseHexSize(const std::string &line)
{
    size_t result;
    size_t i;

    result = 0;
    i = 0;
    while (i < line.length())
    {
        char c = line[i];
        if (c >= '0' && c <= '9')
            result = result * 16 + (c - '0');
        else if (c >= 'a' && c <= 'f')
            result = result * 16 + (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            result = result * 16 + (c - 'A' + 10);
        else
            break;
        i++;
    }
    return (result);
}

std::string decodeChunkedBody(const std::string &body)
{
    std::string result;
    size_t pos;
    size_t chunkSize;
    size_t i;

    i = 0;
    while (i < body.length())
    {
        pos = body.find('\n', i);
        if (pos == std::string::npos)
            break;
        chunkSize = parseHexSize(body.substr(i, pos - i));
        if (chunkSize == 0)
            break;
        i = pos + 1;
        if (i + chunkSize <= body.length())
            result.append(body.substr(i, chunkSize));
        i = i + chunkSize;
        while (i < body.length() && (body[i] == '\r' || body[i] == '\n'))
            i++;
    }
    return (result);
}
