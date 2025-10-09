/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/10/09 19:48:09 by emgul            ###   ########.fr       */
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

int createListeningSocket(const std::string &host, int port)
{
    int fd;

    fd = createSocket();
    if (fd == -1)
        return (-1);
    if (setReuseaddr(fd) == -1 || setNonblocking(fd) == -1 || bindServerSocket(fd, host, port, BACKLOG) == -1)
    {
        close(fd);
        return (-1);
    }
    std::cout << "Server listening on ";
    if (host.empty())
        std::cout << "0.0.0.0";
    else
        std::cout << host;
    std::cout << ":" << port << " (fd: " << fd << ")" << std::endl;
    return (fd);
}
