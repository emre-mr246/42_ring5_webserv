/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirectHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 12:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sstream>

const LocationConfig *findLocationForPath(const std::string &path,
                                          const ServerConfig *server)
{
    size_t i;
    size_t bestMatchLength;
    const LocationConfig *bestMatch;

    if (!server)
        return (NULL);
    bestMatch = NULL;
    bestMatchLength = 0;
    i = 0;
    while (i < server->locations.size())
    {
        if (path.find(server->locations[i].path) == 0)
        {
            if (server->locations[i].path.length() > bestMatchLength)
            {
                bestMatch = &server->locations[i];
                bestMatchLength = server->locations[i].path.length();
            }
        }
        i++;
    }
    return (bestMatch);
}

HttpResponse createRedirectResponse(const LocationConfig *location)
{
    HttpResponse response;

    response.statusCode = location->redirectCode;
    if (location->redirectCode == 301)
        response.statusMessage = "Moved Permanently";
    else if (location->redirectCode == 302)
        response.statusMessage = "Found";
    else if (location->redirectCode == 307)
        response.statusMessage = "Temporary Redirect";
    else
        response.statusMessage = "Redirect";
    response.body = "";
    response.headers["Location"] = location->redirectUrl;
    response.headers["Content-Length"] = "0";
    return (response);
}

int checkLocationRedirect(const HttpRequest &req, const ServerConfig *server,
                          HttpResponse &response)
{
    const LocationConfig *location;

    location = findLocationForPath(req.uri, server);
    if (!location || location->redirectUrl.empty())
        return (0);
    response = createRedirectResponse(location);
    return (1);
}
