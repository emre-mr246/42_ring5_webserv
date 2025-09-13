/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/13 08:21:18 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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

int create_listening_socket()
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