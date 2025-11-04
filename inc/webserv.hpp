/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define DEBUG_MODE 0
#define DEBUG_REQUESTS 0
#define DEBUG_RESPONSES 0

struct HttpRequest;
struct HttpResponse;

#include "config.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BACKLOG 512
#define CLIENT_TIMEOUT 1800
#define CGI_TIMEOUT 1800
#define MAX_REQUEST_BUFFER 134217728
#define DEFAULT_BODY_SIZE 1048576
#define CGI_CHUNK_SIZE 16777216
#define CGI_BUFFER_SIZE 4194304
#define CGI_OUTPUT_MAX_SIZE 134217728
#define FILE_READ_BUFFER_SIZE 65536
#define SELECT_TIMEOUT_USEC 10000
#define POLL_TIMEOUT_MSEC 100

struct ClientRequestBuffer
{
    std::string buffer;
    int headersParsed;
    size_t expectedBodySize;
    int serverFd;
    int needsFullBody;

    ClientRequestBuffer() : buffer(), headersParsed(0), expectedBodySize(0), serverFd(-1), needsFullBody(0) {}
};

void printError(const std::string &context);
std::string &getClientBuffer(int clientFd);
void clearClientBuffer(int clientFd);
void resetClientState(int clientFd);
std::map<std::string, std::string>::const_iterator findHeaderCaseInsensitive(
    const std::map<std::string, std::string> &headers,
    const std::string &headerName);
int areHeadersParsed(int clientFd);
int isRequestComplete(int clientFd);
int getClientServerFd(int clientFd);
void setClientServerFd(int clientFd, int serverFd);
int clientNeedsFullBody(int clientFd);
size_t parseContentLength(const std::string &headers);
size_t findHeaderEnd(const std::string &buffer);
int createListeningSocket(const std::string &host, int port);
int bindServerSocket(int fd, const std::string &host, int port, int backlog, int family);
int setNonblocking(int fd);
void addUniqueAddress(std::vector<std::pair<std::string, int> > &addresses,
                      const std::string &host, int port);
void gatherAddresses(const Config &config,
                     std::vector<std::pair<std::string, int> > &addresses);
int openServerSockets(const std::vector<std::pair<std::string, int> > &addresses,
                      std::vector<int> &fds);
int getPortFromServerFd(int serverFd);
void initPollServer(std::vector<struct pollfd> &pollFds,
                    const std::vector<int> &serverFds);
void addClientToPoll(std::vector<struct pollfd> &pollFds, int clientFd);
void removeClientFromPoll(std::vector<struct pollfd> &pollFds, size_t index);
void updateClientEvents(std::vector<struct pollfd> &pollFds, int clientFd,
                        short events);
int waitForEvents(std::vector<struct pollfd> &pollFds);
int acceptClientConnection(int serverFd);
void handleNewConnection(std::vector<struct pollfd> &pollFds, int serverFd);
int readFromClient(int clientFd, std::vector<struct pollfd> &pollFds,
                   const Config *config);
int processNewData(int clientFd, std::vector<struct pollfd> &pollFds,
                   const Config *config, int wasHeadersParsed);
void handleParseError(const char *buf, ssize_t bytesRead, int clientFd,
                      std::vector<struct pollfd> &pollFds);
size_t getMaxBodySize(const HttpRequest &req, const Config *config);
int checkBodySizeLimit(const HttpRequest &req, const Config *config);
size_t parseContentLengthValue(const std::string &value);
HttpResponse routeRequest(const HttpRequest &req, const Config *config);
bool shouldCloseAfterResponse(const HttpRequest &req);
void handleRequest(const HttpRequest &req, int clientFd,
                   std::vector<struct pollfd> &pollFds, const Config *config);
void sendErrorAndStop(int clientFd, std::vector<struct pollfd> &pollFds,
                      int statusCode, int *shouldContinue);
void checkHeadersForBodySize(int clientFd, std::vector<struct pollfd> &pollFds,
                             const Config *config, int *shouldContinue);
void parseAndHandleRequest(const char *buf, ssize_t bytesRead, int clientFd,
                           std::vector<struct pollfd> &pollFds,
                           const Config *config);
int handleClientData(std::vector<struct pollfd> &pollFds, size_t clientIndex,
                     const Config *config);
int handleClientWrite(std::vector<struct pollfd> &pollFds, size_t clientIndex);
void *memset(void *str, int c, size_t n);
std::string strtrim(const std::string &str);
long strtol(const char *ptr, char **endptr);
int isspace(int c);
void eventLoop(const std::vector<int> &serverFds, const Config *config);
void updateClientTime(int clientFd);
void removeClientTime(int clientFd);
void checkClientTimeouts(std::vector<struct pollfd> &pollFds);
void trackCgiProcess(pid_t pid);
int checkCgiTimeout(pid_t pid);
std::map<int, ClientRequestBuffer> &getClientBuffers(void);
