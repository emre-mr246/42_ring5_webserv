/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:34:46 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int create_socket()
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        print_error("socket(SOCK_STREAM)");
    return (fd);
}

static int set_reuseaddr(int fd)
{
    int opt;

    opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        print_error("setsockopt(SO_REUSEADDR)");
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
        print_error("fcntl(F_GETFL)");
        return -(1);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        print_error("fcntl(F_SETFL)");
        return -(1);
    }
    return (0);
}

static int bind_socket(int fd)
{
    struct sockaddr_in address;
    std::ostringstream oss;

    ft_memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        print_error("bind()");
        return (-1);
    }
    return (0);
}

static int listen_socket(int fd, int backlog)
{
    if (listen(fd, backlog) == -1)
    {
        print_error("listen()");
        return (-1);
    }
    return (0);
}

static int bind_and_listen(int fd, int backlog)
{
    if (bind_socket(fd) == -1)
        return (-1);
    if (listen_socket(fd, backlog) == -1)
        return (-1);
    return (0);
}

int create_listening_socket()
{
    int fd;

    fd = create_socket();
    if (fd == -1)
        return (-1);
    if (set_reuseaddr(fd) == -1 || set_nonblocking(fd) == -1 || bind_and_listen(fd, BACKLOG) == -1)
    {
        close(fd);
        return (-1);
    }
    return (fd);
}
