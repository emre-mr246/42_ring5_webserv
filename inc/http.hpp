/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:36 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 14:45:41 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>

struct HttpRequest
{
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HttpResponse
{
    int statusCode;
    std::string statusMessage;
    std::map<std::string, std::string> headers;
    std::string body;
};

int parseRequestLine(const std::string &line, HttpRequest &req);
int parseHeader(const std::string &line, HttpRequest &req);
int processRequestData(const char *data, size_t len, HttpRequest &req);
std::string buildHttpResponse(HttpResponse &res);
void printHttpRequest(const HttpRequest &req);
std::string decodeChunkedBody(const std::string &body);
int validateHttpRequest(const HttpRequest &req);
HttpResponse createErrorResponse(int statusCode);
int sendResponseToClient(int clientFd, const std::string &response);
std::string resolveFilePath(const std::string &uri);
int readFile(const std::string &path, std::string &content);
int writeFile(const std::string &path, const std::string &content);
std::string getMimeType(const std::string &path);
HttpResponse handleGetRequest(const std::string &uri);
HttpResponse handlePostRequest(const HttpRequest &req);
