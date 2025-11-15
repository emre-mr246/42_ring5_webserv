/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:36 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"
#include <map>
#include <string>
#include <sys/select.h>
#include <vector>

struct HttpRequest
{
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    int clientFd;
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
    bool shouldClose;
};

struct ResponseQueueEntry
{
    std::string data;
    bool shouldClose;
};

struct CgiSelectState
{
    int inputFd;
    int outputFd;
    int inputDone;
    size_t totalWritten;
    const std::string *bodyData;
    std::string *output;
};

int parseRequestLine(const std::string &line, HttpRequest &req);
int parseHeader(const std::string &line, HttpRequest &req);
int processRequestData(const char *data, size_t len, HttpRequest &req);
std::string buildHttpResponse(HttpResponse &res);
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
const ServerConfig *findServerByHost(const Config *config, const std::string &host, int port);
std::string stripPortFromHost(const std::string &host);
const LocationConfig *findLocation(const HttpRequest &req, const Config *config);
const ServerConfig *findServerForRequest(const HttpRequest &req, const Config *config);
void resolveRoot(const LocationConfig *location, const ServerConfig *server, std::string &root);
std::string getHostFromRequest(const HttpRequest &req);
int getPortFromHostHeader(const HttpRequest &req);
const ServerConfig *findServerByPort(const Config *config, int port);
const ServerConfig *findFirstServer(const Config *config);
const LocationConfig *getDefaultLocation(const Config *config);
std::string stripQueryString(const std::string &uri);
int isUriMatchingLocation(const std::string &uri, const std::string &locationPath);
const LocationConfig *findBestMatchingLocation(const ServerConfig *server, const std::string &cleanUri, const std::string &method = "");
int readFile(const std::string &path, std::string &content);
int writeFile(const std::string &path, const std::string &content);
std::string getMimeType(const std::string &path);
HttpResponse handleGetRequest(const HttpRequest &req, const Config *config);
HttpResponse handlePostRequest(const HttpRequest &req, const Config *config);
std::string generateAutoindex(const std::string &dirPath, const std::string &requestUri);
HttpResponse handleDeleteRequest(const HttpRequest &req, const Config *config);
HttpResponse handlePutRequest(const HttpRequest &req, const Config *config);
void setPendingResponse(int clientFd, const std::string &response, bool shouldClose = false);
int hasPendingResponse(int clientFd);
int sendPendingResponse(int clientFd);
void clearPendingResponse(int clientFd);
bool shouldCloseConnection(int clientFd);
std::map<int, PendingResponse> &getPendingResponses(void);
std::map<int, std::vector<ResponseQueueEntry> > &getResponseQueues(void);
HttpResponse executeCgiScript(const std::string &scriptPath, const std::string &interpreter, const HttpRequest &req, const Config *config);
std::string getCgiInterpreter(const std::string &uri, const HttpRequest &req, const Config *config);
std::string extractQueryString(const std::string &uri);
std::string readCgiOutput(int fd);
HttpResponse parseCgiOutput(const std::string &output);
HttpResponse runCgiProcess(int inputPipe[2], int outputPipe[2], const std::string &scriptPath, const std::string &interpreter, const HttpRequest &req, const Config *config);
int isMethodAllowed(const HttpRequest &req, const Config *config);
void parseHeadersFromCgi(const std::string &headers, HttpResponse &response);
void processStatusHeaderFromCgi(HttpResponse &response);
void buildCgiEnvironment(std::vector<std::string> &envStrings, const std::string &queryStr,
                         const HttpRequest &req);
void convertEnvToCharArray(const std::vector<std::string> &envStrings,
                           std::vector<char *> &envVec);
int handleCgiRequest(const std::string &filePath, const HttpRequest &req,
                     const Config *config, HttpResponse &response);
pid_t cgiChildFork(int inputPipe[2], int outputPipe[2],
                   const std::string &scriptPath,
                   const std::string &interpreter,
                   const std::string &queryStr,
                   const HttpRequest &req);
void waitForCgiProcess(pid_t pid, int &status);
int cgiIOSetupPipes(int inputPipe[2], int outputPipe[2]);
int cgiIOProcessSelectEvents(fd_set &readSet, fd_set &writeSet, CgiSelectState &state);
std::string cgiIOHandleWithSelect(int inputFd, int outputFd, pid_t pid,
                                  const std::string &bodyData, int &status);
void cgiSelectRun(CgiSelectState &state, pid_t pid);
