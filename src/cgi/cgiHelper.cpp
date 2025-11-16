/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
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
    location = findLocation(req, config);
    if (!location)
        return ("");
    it = location->cgiPass.find(ext);
    if (it != location->cgiPass.end())
        return (it->second);
    return ("");
}

std::string extractQueryString(const std::string &uri)
{
    size_t qPos;

    qPos = uri.find('?');
    if (qPos == std::string::npos)
        return ("");
    return (uri.substr(qPos + 1));
}

static void addBasicEnv(std::vector<std::string> &envStrings, const std::string &queryStr,
                        const HttpRequest &req)
{
    std::ostringstream oss;

    oss << req.body.length();
    envStrings.push_back("QUERY_STRING=" + queryStr);
    envStrings.push_back("CONTENT_LENGTH=" + oss.str());
    envStrings.push_back("REQUEST_METHOD=" + req.method);
    envStrings.push_back("SERVER_PROTOCOL=" + req.version);
    envStrings.push_back("PATH_INFO=" + req.uri);
}

static void normalizeHeaderName(std::string &headerName)
{
    size_t pos;

    pos = 0;
    while (pos < headerName.length())
    {
        if (headerName[pos] >= 'a' && headerName[pos] <= 'z')
            headerName[pos] = headerName[pos] - 32;
        if (headerName[pos] == '-')
            headerName[pos] = '_';
        pos++;
    }
}

static void addHeaderEnv(std::vector<std::string> &envStrings, const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;
    std::string headerName;

    it = req.headers.begin();
    while (it != req.headers.end())
    {
        headerName = it->first;
        normalizeHeaderName(headerName);
        envStrings.push_back("HTTP_" + headerName + "=" + it->second);
        it++;
    }
}

void buildCgiEnvironment(std::vector<std::string> &envStrings, const std::string &queryStr,
                         const HttpRequest &req)
{
    addBasicEnv(envStrings, queryStr, req);
    addHeaderEnv(envStrings, req);
}

void convertEnvToCharArray(const std::vector<std::string> &envStrings,
                           std::vector<char *> &envVec)
{
    size_t i;
    char *envStr;

    i = 0;
    while (i < envStrings.size())
    {
        envStr = new char[envStrings[i].length() + 1];
        envStrings[i].copy(envStr, envStrings[i].length());
        envStr[envStrings[i].length()] = '\0';
        envVec.push_back(envStr);
        i++;
    }
    envVec.push_back(NULL);
}
