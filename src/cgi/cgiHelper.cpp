/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static std::string getExtension(const std::string &uri)
{
    size_t dotPos;

    dotPos = uri.find_last_of('.');
    if (dotPos == std::string::npos)
        return ("");
    return (uri.substr(dotPos));
}

std::string getCgiInterpreter(const std::string &uri, const HttpRequest &req,
                              const Config *config)
{
    std::string ext;
    const LocationConfig *location;
    std::map<std::string, std::string>::const_iterator it;

    ext = getExtension(uri);
    if (ext.empty())
        return ("");
    location = getLocationConfig(req, config);
    if (!location)
        return ("");
    it = location->cgiPass.find(ext);
    if (it != location->cgiPass.end())
        return (it->second);
    return ("");
}
