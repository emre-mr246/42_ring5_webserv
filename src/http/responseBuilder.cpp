/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 14:45:41 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sstream>

static std::string getStatusMessage(int code)
{
    if (code == 200)
        return ("OK");
    if (code == 404)
        return ("Not Found");
    if (code == 500)
        return ("Internal Server Error");
    if (code == 403)
        return ("Forbidden");
    return ("Unknown");
}

static std::string buildStatusLine(const HttpResponse &res)
{
    std::ostringstream oss;

    oss << "HTTP/1.1 " << res.statusCode << " ";
    oss << res.statusMessage << "\r\n";
    return (oss.str());
}

static std::string buildHeaders(const HttpResponse &res)
{
    std::ostringstream oss;
    std::map<std::string, std::string>::const_iterator it;

    it = res.headers.begin();
    while (it != res.headers.end())
    {
        oss << it->first << ": " << it->second << "\r\n";
        it++;
    }
    oss << "\r\n";
    return (oss.str());
}

std::string buildHttpResponse(HttpResponse &res)
{
    std::ostringstream oss;

    if (res.statusMessage.empty())
        res.statusMessage = getStatusMessage(res.statusCode);
    oss << buildStatusLine(res);
    oss << buildHeaders(res);
    oss << res.body;
    return (oss.str());
}
