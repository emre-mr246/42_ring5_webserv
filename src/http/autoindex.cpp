/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 11:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <dirent.h>
#include <sstream>

static void addHtmlHeader(std::string &html, const std::string &path)
{
    html += "<!DOCTYPE html>\r\n";
    html += "<html>\r\n";
    html += "<head>\r\n";
    html += "<title>Index of ";
    html += path;
    html += "</title>\r\n";
    html += "</head>\r\n";
    html += "<body>\r\n";
    html += "<h1>Index of ";
    html += path;
    html += "</h1>\r\n";
    html += "<hr>\r\n";
    html += "<ul>\r\n";
}

static void addHtmlFooter(std::string &html)
{
    html += "</ul>\r\n";
    html += "<hr>\r\n";
    html += "</body>\r\n";
    html += "</html>\r\n";
}

std::string generateAutoindex(const std::string &dirPath, const std::string &requestUri)
{
    DIR *dir;
    struct dirent *entry;
    std::string html;
    std::string displayUri;

    addHtmlHeader(html, requestUri);
    dir = opendir(dirPath.c_str());
    if (!dir)
        return ("");
    entry = readdir(dir);
    while (entry)
    {
        if (entry->d_name[0] != '.')
        {
            displayUri = requestUri;
            if (displayUri[displayUri.length() - 1] != '/')
                displayUri += "/";
            displayUri += entry->d_name;
            html += "<li><a href=\"";
            html += displayUri;
            html += "\">";
            html += entry->d_name;
            html += "</a></li>\r\n";
        }
        entry = readdir(dir);
    }
    closedir(dir);
    addHtmlFooter(html);
    return (html);
}
