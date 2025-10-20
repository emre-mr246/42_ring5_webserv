/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:36 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"
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
    int serverFd;
};

struct HttpResponse
{
    int statusCode;
    std::string statusMessage;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct PendingResponse
{
    std::string data;
    size_t offset;
};

int parseRequestLine(const std::string &line, HttpRequest &req);
int parseHeader(const std::string &line, HttpRequest &req);
int processRequestData(const char *data, size_t len, HttpRequest &req);
std::string buildHttpResponse(HttpResponse &res);
void printHttpRequest(const HttpRequest &req);
std::string decodeChunkedBody(const std::string &body);
int validateHttpRequest(const HttpRequest &req);
std::string getErrorMessage(int code);
std::string buildErrorPath(const std::string &errorPage,
                           const std::string &root);
int tryLocationErrorPage(int statusCode, const HttpRequest &req,
                         const Config *config, std::string &content);
int tryServerErrorPage(int statusCode, const HttpRequest &req,
                       const Config *config, std::string &content);
HttpResponse buildCustomError(int code, const std::string &content);
HttpResponse createErrorResponse(int statusCode);
HttpResponse createErrorResponse(int statusCode, const HttpRequest &req,
                                 const Config *config);
int sendResponseToClient(int clientFd, std::string &response, size_t &offset);
std::string resolveFilePath(const std::string &uri, const HttpRequest &req,
                            const Config *config);
void getLocationSettings(const HttpRequest &req, const Config *config,
                         std::string &root, std::string &index);
const LocationConfig *getLocationConfig(const HttpRequest &req,
                                        const Config *config);
const ServerConfig *findServerByHost(const Config *config, const std::string &host, int port);
std::string stripPortFromHost(const std::string &host);
const LocationConfig *findLocation(const HttpRequest &req, const Config *config);
const ServerConfig *findServerForRequest(const HttpRequest &req, const Config *config);
void resolveRoot(const LocationConfig *location, const ServerConfig *server, std::string &root);
std::string getHostFromRequest(const HttpRequest &req);
int getPortFromHostHeader(const HttpRequest &req);
const ServerConfig *findServerByPort(const Config *config, int port);
const LocationConfig *getDefaultLocation(const Config *config);
std::string stripQueryString(const std::string &uri);
int isUriMatchingLocation(const std::string &uri, const std::string &locationPath);
const LocationConfig *findBestMatchingLocation(const ServerConfig *server, const std::string &cleanUri);
int readFile(const std::string &path, std::string &content);
int writeFile(const std::string &path, const std::string &content);
std::string getMimeType(const std::string &path);
HttpResponse handleGetRequest(const HttpRequest &req, const Config *config);
HttpResponse handlePostRequest(const HttpRequest &req, const Config *config);
HttpResponse handleDeleteRequest(const HttpRequest &req, const Config *config);
void setPendingResponse(int clientFd, const std::string &response);
int hasPendingResponse(int clientFd);
int sendPendingResponse(int clientFd);
void clearPendingResponse(int clientFd);
HttpResponse executeCgiScript(const std::string &scriptPath, const std::string &interpreter, const HttpRequest &req, const Config *config);
std::string getCgiInterpreter(const std::string &uri, const HttpRequest &req, const Config *config);
char *createEnvString(const std::string &queryStr);
std::string extractQueryString(const std::string &uri);
std::string readCgiOutput(int fd);
HttpResponse parseCgiOutput(const std::string &output);
HttpResponse runCgiProcess(int pipefd[2], const std::string &scriptPath, const std::string &interpreter, const HttpRequest &req, const Config *config);
int isMethodAllowed(const HttpRequest &req, const Config *config);
void parseHeadersFromCgi(const std::string &headers, HttpResponse &response);
void processStatusHeaderFromCgi(HttpResponse &response);
