/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestPrinter.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static void displayRequestLine(const HttpRequest &req)
{
    std::cout << "Method: " << req.method << std::endl;
    std::cout << "URI: " << req.uri << std::endl;
    std::cout << "Version: " << req.version << std::endl;
}

static void displayHeaders(const HttpRequest &req)
{
    std::map<std::string, std::string>::const_iterator it;

    std::cout << "Headers:" << std::endl;
    it = req.headers.begin();
    while (it != req.headers.end())
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
        it++;
    }
}

void printHttpRequest(const HttpRequest &req)
{
    std::cout << "=== HTTP Request ===" << std::endl;
    displayRequestLine(req);
    displayHeaders(req);
    if (!req.body.empty())
        std::cout << "Body: " << req.body << std::endl;
    std::cout << "===================" << std::endl;
}
