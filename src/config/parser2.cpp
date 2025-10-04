/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:14:28 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 13:21:54 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void parseLocationDirective(const std::string &line, LocationConfig &location)
{
    std::string trimmed;

    trimmed = strtrim(line);
    if (trimmed.find("client_max_body_size") == 0)
        location.client_max_body_size = extractBodySize(line);
    else if (trimmed.find("accepted_methods") == 0)
        location.accepted_methods = extractAcceptedMethods(line);
    else if (trimmed.find("root") == 0)
        location.root = extractRoot(line);
    else if (trimmed.find("index") == 0)
        location.index_file = extractIndex(line);
    else if (trimmed.find("autoindex") == 0)
        location.autoindex = extractAutoindex(line);
    else if (trimmed.find("upload_path") == 0)
        location.upload_path = extractUploadPath(line);
    else if (trimmed.find("redirect") == 0)
        location.redirect = extractRedirect(line);
    else if (trimmed.find("cgi_pass") == 0)
        location.cgi_pass = extractCgiPass(line);
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
        server.server_names = extractServerNames(line);
    else if (trimmed.find("client_max_body_size") == 0)
        server.client_max_body_size = extractBodySize(line);
    else if (trimmed.find("error_page") == 0)
        addErrorPages(extractErrorPageValue(line), server);
}

int isValidLocationDirective(const std::string &line, int depth, bool inLocation)
{
    if (depth < 1 || !inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}

int isValidServerDirective(const std::string &line, int depth, bool inLocation)
{
    if (depth < 0 || inLocation)
        return (0);
    if (isServerBlock(line) || isLocationBlock(line))
        return (0);
    return (1);
}
