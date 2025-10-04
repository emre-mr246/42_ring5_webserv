/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:49:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 21:29:35 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void addMethodToList(const std::string &value, size_t &start, std::vector<std::string> &methods)
{
    size_t end;
    std::string method;

    end = value.find(' ', start);
    if (end == std::string::npos)
        end = value.length();
    method = strtrim(value.substr(start, end - start));
    if (!method.empty())
        methods.push_back(method);
    start = end + 1;
}

std::vector<std::string> extractAcceptedMethods(const std::string &line)
{
    size_t pos;
    size_t start;
    std::string value;
    std::vector<std::string> methods;

    pos = line.find("accepted_methods");
    if (pos == std::string::npos)
        return (methods);
    value = strtrim(line.substr(pos + 16));
    if (!value.empty() && value[value.length() - 1] == ';')
        value = value.substr(0, value.length() - 1);
    start = 0;
    while (start < value.length())
        addMethodToList(value, start, methods);
    return (methods);
}

std::string extractUploadPath(const std::string &line)
{
    std::string value;
    size_t pos;

    pos = line.find("upload_path");
    if (pos == std::string::npos)
        return ("");
    value = strtrim(line.substr(pos + 11));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    return (strtrim(value));
}

std::pair<int, std::string> extractRedirect(const std::string &line)
{
    std::string value;
    std::string codeStr;
    std::string url;
    size_t pos;
    size_t spacePos;
    int code;

    pos = line.find("redirect");
    if (pos == std::string::npos)
        return (std::make_pair(0, ""));
    value = strtrim(line.substr(pos + 8));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    spacePos = value.find(' ');
    if (spacePos == std::string::npos)
        return (std::make_pair(0, ""));
    codeStr = strtrim(value.substr(0, spacePos));
    url = strtrim(value.substr(spacePos + 1));
    code = parseBodySize(codeStr);
    return (std::make_pair(code, url));
}

std::pair<std::string, std::string> extractCgiPass(const std::string &line)
{
    std::string value;
    std::string ext;
    std::string path;
    size_t pos;
    size_t spacePos;

    pos = line.find("cgi_pass");
    if (pos == std::string::npos)
        return (std::make_pair("", ""));
    value = strtrim(line.substr(pos + 8));
    if (!value.empty() && value[value.length() - 1] == ';')
        value.resize(value.length() - 1);
    spacePos = value.find(' ');
    if (spacePos == std::string::npos)
        return (std::make_pair("", ""));
    ext = strtrim(value.substr(0, spacePos));
    path = strtrim(value.substr(spacePos + 1));
    return (std::make_pair(ext, path));
}
