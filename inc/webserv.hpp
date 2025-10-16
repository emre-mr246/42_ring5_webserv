/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

#define PORT 8080
#define BACKLOG 42
#define DEBUG_MODE 1
#define CLIENT_TIMEOUT 60
#define CGI_TIMEOUT 60

void printError(const std::string &context);
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
