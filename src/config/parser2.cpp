/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void parseLocationDirective(const std::string &line, LocationConfig &location)
{
    std::string trimmed;

    trimmed = strtrim(line);
    if (trimmed.find("client_max_body_size") == 0)
        location.clientMaxBodySize = extractBodySize(line);
    else if (trimmed.find("accepted_methods") == 0)
        location.acceptedMethods = extractAcceptedMethods(line);
    else if (trimmed.find("root") == 0)
        location.root = extractRoot(line);
    else if (trimmed.find("index") == 0)
        location.indexFile = extractIndex(line);
    else if (trimmed.find("autoindex") == 0)
        location.autoindex = extractAutoindex(line);
    else if (trimmed.find("cgi_pass") == 0)
        addCgiPass(line, location);
    else if (trimmed.find("error_page") == 0)
        addErrorPages(extractErrorPageValue(line), location);
    else if (trimmed.find("return") == 0)
        parseRedirectDirective(line, location);
}

void parseServerDirective(const std::string &line, ServerConfig &server)
{
    std::string trimmed;
    std::string value;

    trimmed = strtrim(line);
    if (trimmed.find("listen") == 0)
    {
        value = extractListenValue(line);
        addListenAddress(value, server);
    }
    else if (trimmed.find("server_name") == 0)
        server.serverNames = extractServerNames(line);
    else if (trimmed.find("root") == 0)
        server.root = extractRoot(line);
    else if (trimmed.find("client_max_body_size") == 0)
        server.clientMaxBodySize = extractBodySize(line);
    else if (trimmed.find("error_page") == 0)
        addErrorPages(extractErrorPageValue(line), server);
}

int isValidLocationDirective(const std::string &line, int depth,
                             bool inLocation)
{
    if (depth < 1 || !inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}

int isValidServerDirective(const std::string &line, int depth,
                           bool inLocation)
{
    if (depth < 0 || inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}
