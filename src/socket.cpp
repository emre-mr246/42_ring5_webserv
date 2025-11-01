/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:29 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int createSocket(int family)
{
    int fd;

    fd = socket(family, SOCK_STREAM, 0);
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

int setNonblocking(int fd)
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

int createListeningSocket(const std::string &host, int port)
{
    int fd;
    int family;

    family = AF_INET;
    fd = createSocket(family);
    if (fd == -1)
        return (-1);
    if (setReuseaddr(fd) == -1 || setNonblocking(fd) == -1 ||
        bindServerSocket(fd, host, port, BACKLOG, family) == -1)
    {
        close(fd);
        return (-1);
    }
    return (fd);
}
