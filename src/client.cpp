/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/09/15 15:22:19 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int acceptClient(int server_fd)
{
    int client_fd;
    int flags;

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (-1);
        printError("accept()");
        return (-1);
    }
    flags = fcntl(client_fd, F_GETFL, 0);
    if (flags != -1)
        fcntl(client_fd, F_SETFL, flags & ~O_NONBLOCK);
    return (client_fd);
}

void readAndPrintClient(int client_fd)
{
    char buf[4096];
    ssize_t n;

    while (1)
    {
        n = read(client_fd, buf, sizeof(buf));
        if (n > 0)
        {
            std::cout << "[client " << client_fd << "] ";
            std::cout.write(buf, n);
            if (buf[n - 1] == '\n')
                std::cout.flush();
            continue;
        }
        if (n == 0)
        {
            std::cout.flush();
            break;
        }
        if (errno == EINTR)
            continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            break;
        printError("read(client)");
        break;
    }
}

int readClientData(int client_fd)
{
    char buf[4096];
    ssize_t n;

    n = read(client_fd, buf, sizeof(buf));
    if (n > 0)
    {
        std::cout << "[client " << client_fd << "] ";
        std::cout.write(buf, n);
        if (buf[n - 1] == '\n')
            std::cout.flush();
        return (1);
    }
    if (n == 0)
    {
        std::cout.flush();
        return (0);
    }
    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        return (1);
    printError("read(client)");
    return (0);
}
