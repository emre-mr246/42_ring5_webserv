/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:46 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct HttpRequest;
struct HttpResponse;

#include "config.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BACKLOG 4096
#define CLIENT_TIMEOUT 300
#define CGI_TIMEOUT 30000
#define MAX_REQUEST_BUFFER 134217728
#define DEFAULT_BODY_SIZE 1048576
#define CGI_CHUNK_SIZE 16777216
#define CGI_BUFFER_SIZE 8388608
#define CGI_OUTPUT_MAX_SIZE 134217728
#define FILE_READ_BUFFER_SIZE 262144
#define BUFFER_RESERVE_SIZE 8192
#define SELECT_TIMEOUT_USEC 1000
#define POLL_TIMEOUT_MSEC 10

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct addrinfo addrinfo;
typedef struct timeval timeval;
typedef struct pollfd pollfd;
typedef struct stat stat_t;

struct ClientRequestBuffer
{
    std::string buffer;
    int headersParsed;
    size_t expectedBodySize;
    int serverFd;

    ClientRequestBuffer() : buffer(), headersParsed(0), expectedBodySize(0), serverFd(-1) {}
};

void printError(const std::string &context);
std::string &getClientBuffer(int clientFd);
void clearClientBuffer(int clientFd);
void resetClientState(int clientFd);
int areHeadersParsed(int clientFd);
int isRequestComplete(int clientFd);
int getClientServerFd(int clientFd);
void setClientServerFd(int clientFd, int serverFd);
size_t parseContentLength(const std::string &headers);
size_t findHeaderEnd(const std::string &buffer);
int createListeningSocket(const std::string &host, int port);
int bindServerSocket(int fd, const std::string &host, int port, int backlog);
int setNonblocking(int fd);
int getAddressInfo(const std::string &host, int port, addrinfo **res);
int tryBindToAddresses(int fd, addrinfo *res);
int startListening(int fd, int backlog);
void initializeSignals(void);
int getShutdownFlag(void);
std::string getConfigPath(int argc, char **argv);
void cleanupSockets(const std::vector<int> &serverFds);
void addToAddressList(AddressList &addresses,
                      const std::string &host, int port);
void gatherAddresses(const Config &config,
                     AddressList &addresses);
int openServerSockets(const AddressList &addresses,
                      std::vector<int> &fds);
int getPortFromServerFd(int serverFd);
void initPollServer(std::vector<pollfd> &pollFds,
                    const std::vector<int> &serverFds);
void addClientToPoll(std::vector<pollfd> &pollFds, int clientFd);
void removeClientFromPoll(std::vector<pollfd> &pollFds, size_t index);
void updateClientEvents(std::vector<pollfd> &pollFds, int clientFd,
                        short events);
int waitForEvents(std::vector<pollfd> &pollFds);
int acceptClientConnection(int serverFd);
void handleNewConnection(std::vector<pollfd> &pollFds, int serverFd);
int readFromClient(int clientFd, std::vector<pollfd> &pollFds,
                   const Config *config);
int processNewData(int clientFd, std::vector<pollfd> &pollFds,
                   const Config *config, int wasHeadersParsed);
void handleParseError(const char *buf, ssize_t bytesRead, int clientFd,
                      std::vector<pollfd> &pollFds);
size_t getMaxBodySize(const HttpRequest &req, const Config *config);
int checkBodySizeLimit(const HttpRequest &req, const Config *config);
size_t parseContentLengthValue(const std::string &value);
HttpResponse routeRequest(const HttpRequest &req, const Config *config);
bool shouldCloseAfterResponse(const HttpRequest &req);
void handleRequest(const HttpRequest &req, int clientFd,
                   std::vector<pollfd> &pollFds, const Config *config);
void sendErrorAndStop(int clientFd, std::vector<pollfd> &pollFds,
                      int statusCode, int *shouldContinue);
void checkHeadersForBodySize(int clientFd, std::vector<pollfd> &pollFds,
                             const Config *config, int *shouldContinue);
void parseAndHandleRequest(const char *buf, ssize_t bytesRead, int clientFd,
                           std::vector<pollfd> &pollFds,
                           const Config *config);
int handleClientData(std::vector<pollfd> &pollFds, size_t clientIndex,
                     const Config *config);
int handleClientWrite(std::vector<pollfd> &pollFds, size_t clientIndex);
void *memset(void *str, int c, size_t n);
std::string strtrim(const std::string &str);
long strtol(const char *ptr, char **endptr);
int isspace(int c);
void eventLoop(const std::vector<int> &serverFds, const Config *config);
std::map<int, ClientRequestBuffer> &getClientBuffers(void);
void cleanupClientBuffers(void);
void cleanupServerFdMap(void);
void cleanupResponseQueues(void);
