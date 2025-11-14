/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:31 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sstream>

static std::string buildErrorBody(int code, const std::string &message)
{
    std::ostringstream oss;

    oss << "<html><head><title>" << code << " " << message << "</title></head>";
    oss << "<body><h1>" << code << " " << message << "</h1></body></html>";
    return (oss.str());
}

HttpResponse createErrorResponse(int statusCode)
{
    HttpResponse response;
    std::ostringstream oss;

    response.statusCode = statusCode;
    response.statusMessage = getErrorMessage(statusCode);
    response.body = buildErrorBody(statusCode, response.statusMessage);
    oss << response.body.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = "text/html";
    return (response);
}

HttpResponse buildCustomError(int code, const std::string &content)
{
    HttpResponse response;
    std::ostringstream oss;

    response.statusCode = code;
    response.statusMessage = getErrorMessage(code);
    response.body = content;
    oss << response.body.length();
    response.headers["Content-Length"] = oss.str();
    response.headers["Content-Type"] = "text/html";
    return (response);
}

static std::string getAllowedMethods(const HttpRequest &req, const Config *config)
{
    const LocationConfig *location;
    std::ostringstream oss;
    size_t i;

    location = findLocation(req, config);
    if (!location || location->acceptedMethods.empty())
        return ("");
    i = 0;
    while (i < location->acceptedMethods.size())
    {
        if (i > 0)
            oss << ", ";
        oss << location->acceptedMethods[i];
        i++;
    }
    return (oss.str());
}

HttpResponse createErrorResponse(int statusCode, const HttpRequest &req,
                                 const Config *config)
{
    std::string content;
    HttpResponse response;
    std::string allowHeader;
    int result;

    result = tryLocationErrorPage(statusCode, req, config, content);
    if (result == 1)
        response = buildCustomError(statusCode, content);
    else
    {
        result = tryServerErrorPage(statusCode, req, config, content);
        if (result == 1)
            response = buildCustomError(statusCode, content);
        else
            response = createErrorResponse(statusCode);
    }
    if (statusCode == 405)
    {
        allowHeader = getAllowedMethods(req, config);
        if (!allowHeader.empty())
            response.headers["Allow"] = allowHeader;
    }
    return (response);
}
