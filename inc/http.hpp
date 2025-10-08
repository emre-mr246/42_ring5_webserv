/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:36 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 11:18:27 by emgul            ###   ########.fr       */
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
    int status_code;
    std::string status_message;
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
HttpResponse createErrorResponse(int status_code);
int sendResponseToClient(int client_fd, const std::string &response);
std::string resolveFilePath(const std::string &uri);
int readFile(const std::string &path, std::string &content);
std::string getMimeType(const std::string &path);
HttpResponse handleGetRequest(const std::string &uri);
