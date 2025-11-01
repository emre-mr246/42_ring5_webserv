/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <ctime>
#include <sstream>

static std::string getCurrentDate()
{
    time_t now;
    struct tm *gmtTime;
    char buffer[100];

    now = time(NULL);
    gmtTime = gmtime(&now);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
    return (std::string(buffer));
}

static std::string getStatusMessage(int code)
{
    switch (code)
    {
    case 200:
        return ("OK");
    case 400:
        return ("Bad Request");
    case 403:
        return ("Forbidden");
    case 404:
        return ("Not Found");
    case 405:
        return ("Method Not Allowed");
    case 413:
        return ("Payload Too Large");
    case 500:
        return ("Internal Server Error");
    case 504:
        return ("Gateway Timeout");
    default:
        return ("Unknown");
    }
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
    if (res.headers.find("Connection") == res.headers.end())
        res.headers["Connection"] = "keep-alive";
    if (res.headers.find("Date") == res.headers.end())
        res.headers["Date"] = getCurrentDate();
    oss << buildStatusLine(res);
    oss << buildHeaders(res);
    oss << res.body;
    return (oss.str());
}

std::string buildHttpResponseHeadersOnly(HttpResponse &res)
{
    std::ostringstream oss;

    if (res.statusMessage.empty())
        res.statusMessage = getStatusMessage(res.statusCode);
    if (res.headers.find("Connection") == res.headers.end())
        res.headers["Connection"] = "keep-alive";
    if (res.headers.find("Date") == res.headers.end())
        res.headers["Date"] = getCurrentDate();
    oss << buildStatusLine(res);
    oss << buildHeaders(res);
    return (oss.str());
}
