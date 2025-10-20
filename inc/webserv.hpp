/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct HttpRequest;

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

#define BACKLOG 42
#define CLIENT_TIMEOUT 30
#define CGI_TIMEOUT 15
#define MAX_REQUEST_BUFFER 10485760

struct ClientRequestBuffer
{
    std::string buffer;
    int headersParsed;
    size_t expectedBodySize;
};

void printError(const std::string &context);
std::string &getClientBuffer(int clientFd);
void clearClientBuffer(int clientFd);
int areHeadersParsed(int clientFd);
int isRequestComplete(int clientFd);
size_t parseContentLength(const std::string &headers);
size_t findHeaderEnd(const std::string &buffer);
int createListeningSocket(const std::string &host, int port);
int bindServerSocket(int fd, const std::string &host, int port, int backlog);
int setNonblocking(int fd);
void addUniqueAddress(std::vector<std::pair<std::string, int> > &addresses,
                      const std::string &host, int port);
void gatherAddresses(const Config &config,
                     std::vector<std::pair<std::string, int> > &addresses);
int openServerSockets(const std::vector<std::pair<std::string, int> > &addresses,
                      std::vector<int> &fds);
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
int validateRequestSize(const char *buf, ssize_t bytesRead, int clientFd,
                        std::vector<struct pollfd> &pollFds);
void handleParseError(const char *buf, ssize_t bytesRead, int clientFd,
                      std::vector<struct pollfd> &pollFds);
size_t getMaxBodySize(const HttpRequest &req, const Config *config);
int checkBodySizeLimit(const HttpRequest &req, const Config *config);
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
