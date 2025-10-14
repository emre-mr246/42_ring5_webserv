/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/14 15:25:06 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int hasDangerousPattern(const std::string &uri)
{
    size_t i;

    i = 0;
    while (i < uri.length())
    {
        if (uri[i] == '.' && uri[i + 1] == '.')
            return (1);
        i++;
    }
    return (0);
}

static int isPathSafe(const std::string &uri)
{
    if (hasDangerousPattern(uri))
        return (0);
    if (uri.find("//") != std::string::npos)
        return (0);
    return (1);
}

std::string resolveFilePath(const std::string &uri)
{
    std::string root;
    std::string path;

    root = "./www";
    if (!isPathSafe(uri))
        return ("");
    if (uri == "/")
        path = root + "/index.html";
    else
        path = root + uri;
    return (path);
}
