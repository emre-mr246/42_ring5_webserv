/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimeType.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 12:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static std::string extractExtension(const std::string &path)
{
    size_t pos;

    pos = path.rfind('.');
    if (pos == std::string::npos)
        return ("");
    return (path.substr(pos));
}

static void initMimeMap(std::map<std::string, std::string> &mime_map)
{
    mime_map[".html"] = "text/html";
    mime_map[".htm"] = "text/html";
    mime_map[".css"] = "text/css";
    mime_map[".js"] = "application/javascript";
    mime_map[".txt"] = "text/plain";
    mime_map[".jpg"] = "image/jpeg";
    mime_map[".jpeg"] = "image/jpeg";
    mime_map[".png"] = "image/png";
    mime_map[".gif"] = "image/gif";
    mime_map[".json"] = "application/json";
    mime_map[".xml"] = "application/xml";
    mime_map[".pdf"] = "application/pdf";
}

std::string getMimeType(const std::string &path)
{
    static std::map<std::string, std::string> mime_map;
    static bool initialized = false;
    std::string ext;
    std::map<std::string, std::string>::iterator it;

    if (!initialized)
    {
        initMimeMap(mime_map);
        initialized = true;
    }
    ext = extractExtension(path);
    it = mime_map.find(ext);
    if (it != mime_map.end())
        return (it->second);
    return ("application/octet-stream");
}
