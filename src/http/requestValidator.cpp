/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int isValidMethod(const std::string &method)
{
    if (method == "GET")
        return (1);
    if (method == "POST")
        return (1);
    if (method == "DELETE")
        return (1);
    return (0);
}

static int isValidVersion(const std::string &version)
{
    if (version.find("HTTP/1.1") != std::string::npos)
        return (1);
    if (version.find("HTTP/1.0") != std::string::npos)
        return (1);
    return (0);
}

static int hasNullByte(const std::string &uri)
{
    size_t i;

    i = 0;
    while (i < uri.length())
    {
        if (uri[i] == '\0')
            return (1);
        i++;
    }
    return (0);
}

static int isValidUri(const std::string &uri)
{
    if (uri.empty())
        return (0);
    if (uri[0] != '/')
        return (0);
    if (uri.length() > 2048)
        return (0);
    if (hasNullByte(uri))
        return (0);
    return (1);
}

int validateHttpRequest(const HttpRequest &req)
{
    if (!isValidMethod(req.method))
        return (0);
    if (!isValidUri(req.uri))
        return (0);
    if (!isValidVersion(req.version))
        return (0);
    return (1);
}
