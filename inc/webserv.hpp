/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

void print_error(const std::string &context);

int create_listening_socket();

void init_poll_server(std::vector<struct pollfd> &pollfds, int server_fd);
void add_client_to_poll(std::vector<struct pollfd> &pollfds, int client_fd);
void remove_client_from_poll(std::vector<struct pollfd> &pollfds, size_t index);
int wait_for_events(std::vector<struct pollfd> &pollfds);

int accept_client_connection(int server_fd);
void handle_new_connection(std::vector<struct pollfd> &pollfds, int server_fd);
int read_from_client(int client_fd);
int handle_client_data(std::vector<struct pollfd> &pollfds, size_t client_index);
int read_client_data(int client_fd);

void *ft_memset(void *str, int c, size_t n);
size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);

void event_loop(int server_fd);
