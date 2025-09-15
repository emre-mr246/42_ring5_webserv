/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/09/15 14:30:46 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"
#include <arpa/inet.h>
#include <cerrno>
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

#define PORT 8080
#define BACKLOG 42

void printError(const std::string &context);

int createListeningSocket();

void initPollServer(std::vector<struct pollfd> &pollfds, int server_fd);
void addClientToPoll(std::vector<struct pollfd> &pollfds, int client_fd);
void removeClientFromPoll(std::vector<struct pollfd> &pollfds, size_t index);
int waitForEvents(std::vector<struct pollfd> &pollfds);

int acceptClientConnection(int server_fd);
void handleNewConnection(std::vector<struct pollfd> &pollfds, int server_fd);
int readFromClient(int client_fd);
int handleClientData(std::vector<struct pollfd> &pollfds, size_t client_index);
int readClientData(int client_fd);

void *memset(void *str, int c, size_t n);
size_t strlcpy(char *dst, const char *src, size_t dstsize);
std::string strtrim(const std::string &str);
long strtol(const char *ptr, char **endptr);
int isspace(int c);

void eventLoop(int server_fd);
