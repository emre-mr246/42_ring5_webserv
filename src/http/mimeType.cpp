/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimeType.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:30 by emgul            ###   ########.fr       */
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

static void initMimeMap(std::map<std::string, std::string> &mimeMap)
{
    mimeMap[".html"] = "text/html";
    mimeMap[".htm"] = "text/html";
    mimeMap[".css"] = "text/css";
    mimeMap[".js"] = "application/javascript";
    mimeMap[".txt"] = "text/plain";
    mimeMap[".jpg"] = "image/jpeg";
    mimeMap[".jpeg"] = "image/jpeg";
    mimeMap[".png"] = "image/png";
    mimeMap[".gif"] = "image/gif";
    mimeMap[".json"] = "application/json";
    mimeMap[".xml"] = "application/xml";
    mimeMap[".pdf"] = "application/pdf";
}

std::string getMimeType(const std::string &path)
{
    static std::map<std::string, std::string> mimeMap;
    static bool initialized = false;
    std::string ext;
    std::map<std::string, std::string>::iterator it;

    if (!initialized)
    {
        initMimeMap(mimeMap);
        initialized = true;
    }
    ext = extractExtension(path);
    it = mimeMap.find(ext);
    if (it != mimeMap.end())
        return (it->second);
    return ("application/octet-stream");
}
