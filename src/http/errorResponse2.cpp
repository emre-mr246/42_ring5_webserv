/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorResponse2.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string getErrorMessage(int code)
{
    if (code == 400)
        return ("Bad Request");
    if (code == 403)
        return ("Forbidden");
    if (code == 404)
        return ("Not Found");
    if (code == 405)
        return ("Method Not Allowed");
    if (code == 413)
        return ("Payload Too Large");
    if (code == 500)
        return ("Internal Server Error");
    if (code == 504)
        return ("Gateway Timeout");
    return ("Error");
}

std::string buildErrorPath(const std::string &errorPage,
                           const std::string &root)
{
    std::string path;

    if (errorPage.empty())
        return ("");
    if (errorPage[0] == '/')
        path = root + errorPage;
    else
        path = root + "/" + errorPage;
    return (path);
}

int tryLocationErrorPage(int statusCode, const HttpRequest &req,
                         const Config *config, std::string &content)
{
    const LocationConfig *location;
    const ServerConfig *server;
    std::map<int, std::string>::const_iterator it;
    std::string errorPath;
    std::string root;

    location = findLocation(req, config);
    if (!location || location->errorPages.empty())
        return (0);
    it = location->errorPages.find(statusCode);
    if (it == location->errorPages.end())
        return (0);
    server = findServerForRequest(req, config);
    resolveRoot(location, server, root);
    errorPath = buildErrorPath(it->second, root);
    return (readFile(errorPath, content));
}

int tryServerErrorPage(int statusCode, const HttpRequest &req,
                       const Config *config, std::string &content)
{
    const ServerConfig *server;
    std::string errorPage;
    std::string errorPath;
    std::map<int, std::string>::const_iterator it;

    server = findServerForRequest(req, config);
    if (!server)
        return (0);
    it = server->errorPages.find(statusCode);
    if (it == server->errorPages.end())
        return (0);
    errorPath = buildErrorPath(it->second, server->root);
    return (readFile(errorPath, content));
}
