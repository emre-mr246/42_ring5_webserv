/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/15 14:30:46 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int createSocket()
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        printError("socket(SOCK_STREAM)");
    return (fd);
}

static int setReuseaddr(int fd)
{
    int opt;

    opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        printError("setsockopt(SO_REUSEADDR)");
        return (-1);
    }
    return (0);
}

static int setNonblocking(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        printError("fcntl(F_GETFL)");
        return -(1);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        printError("fcntl(F_SETFL)");
        return -(1);
    }
    return (0);
}

static int bindSocket(int fd)
{
    struct sockaddr_in address;
    std::ostringstream oss;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        printError("bind()");
        return (-1);
    }
    return (0);
}

static int listenSocket(int fd, int backlog)
{
    if (listen(fd, backlog) == -1)
    {
        printError("listen()");
        return (-1);
    }
    return (0);
}

static int bindAndListen(int fd, int backlog)
{
    if (bindSocket(fd) == -1)
        return (-1);
    if (listenSocket(fd, backlog) == -1)
        return (-1);
    return (0);
}

int createListeningSocket()
{
    int fd;

    std::cout << "[INFO] Creating server socket..." << std::endl;
    fd = createSocket();
    if (fd == -1)
        return (-1);
    if (setReuseaddr(fd) == -1 || setNonblocking(fd) == -1 || bindAndListen(fd, BACKLOG) == -1)
    {
        close(fd);
        return (-1);
    }
    return (fd);
}
