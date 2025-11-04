/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestPrinter.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

void printHttpRequest(const HttpRequest &req)
{
    if (DEBUG_REQUESTS)
    {
        std::cout << "[REQUEST] === HTTP Request ===" << std::endl;
        std::cout << "[REQUEST] Method: " << req.method << std::endl;
        std::cout << "[REQUEST] URI: " << req.uri << std::endl;
        std::cout << "[REQUEST] Version: " << req.version << std::endl;
        std::cout << "[REQUEST] Headers:" << std::endl;
        std::map<std::string, std::string>::const_iterator it;
        it = req.headers.begin();
        while (it != req.headers.end())
        {
            std::cout << "[REQUEST]   " << it->first << ": " << it->second << std::endl;
            it++;
        }
        if (!req.body.empty())
            std::cout << "[REQUEST] Body: " << req.body << std::endl;
        std::cout << "[REQUEST] ===================" << std::endl;
        std::cout.flush();
    }
}
