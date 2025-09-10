/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/09/10 11:25:22 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

static int create_socket()
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        log_error("socket()");
    return (fd);
}

static int set_reuseaddr(int fd)
{
    int opt;

    opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        log_error("setsockopt(SO_REUSEADDR)");
        return (-1);
    }
    return (0);
}

static int set_nonblocking(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        log_error("fcntl(F_GETFL)");
        return -(1);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        log_error("fcntl(F_SETFL)");
        return -(1);
    }
    return (0);
}

static int bind_and_listen(int fd, uint16_t port, int backlog)
{
    struct sockaddr_in address;
    std::ostringstream oss;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {

        oss << "bind(port=" << port << ")";
        log_error(oss.str());
        return (-1);
    }
    if (listen(fd, backlog) == -1)
    {
        log_error("listen()");
        return (-1);
    }
    return (0);
}

static int create_listening_socket()
{
    int fd;

    fd = create_socket();
    if (fd == -1)
        return (-1);
    if (set_reuseaddr(fd) == -1)
    {
        close(fd);
        return (-1);
    }
    if (set_nonblocking(fd) == -1)
    {
        close(fd);
        return (-1);
    }
    if (bind_and_listen(fd, PORT, BACKLOG) == -1)
    {
        close(fd);
        return (-1);
    }
    return (fd);
}

static void event_loop(int server_fd)
{
    (void)server_fd;

    while (1)
        pause();
}

int main()
{
    int server_fd;

    server_fd = create_listening_socket();
    if (server_fd == -1)
        return (1);
    event_loop(server_fd);
    close(server_fd);
    return (0);
}
