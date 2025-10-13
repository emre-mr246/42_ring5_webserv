/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:12 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sstream>

static std::string getErrorMessage(int code)
{
    if (code == 400)
        return ("Bad Request");
    if (code == 404)
        return ("Not Found");
    if (code == 405)
        return ("Method Not Allowed");
    if (code == 500)
        return ("Internal Server Error");
    return ("Error");
}

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
