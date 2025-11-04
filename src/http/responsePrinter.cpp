/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePrinter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 15:17:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

void printHttpResponse(const HttpResponse &res)
{
    if (DEBUG_RESPONSES)
    {
        std::cout << "[RESPONSE] === HTTP Response ===" << std::endl;
        std::cout << "[RESPONSE] Status: " << res.statusCode << " " << res.statusMessage << std::endl;
        std::cout << "[RESPONSE] Headers:" << std::endl;
        std::map<std::string, std::string>::const_iterator it;

        it = res.headers.begin();
        while (it != res.headers.end())
        {
            std::cout << "[RESPONSE]   " << it->first << ": " << it->second << std::endl;
            it++;
        }
        if (!res.body.empty())
        {
            std::string bodyPreview;
            if (res.body.length() > 100)
                bodyPreview = res.body.substr(0, 100) + "...";
            else
                bodyPreview = res.body;
            std::cout << "[RESPONSE] Body: " << bodyPreview << std::endl;
        }
        std::cout << "[RESPONSE] ====================" << std::endl;
        std::cout.flush();
    }
}
