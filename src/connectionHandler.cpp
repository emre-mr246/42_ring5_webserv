/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 02:14:26 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int acceptClientConnection(int server_fd)
{
    int client_fd;

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (-1);
        printError("accept()");
        return (-1);
    }
    return (client_fd);
}

void handleNewConnection(std::vector<struct pollfd> &pollfds, int server_fd)
{
    int client_fd;

    client_fd = acceptClientConnection(server_fd);
    if (client_fd == -1)
        return;
    std::cout << "[RaRe Server] client " << client_fd << " connected" << std::endl;
    addClientToPoll(pollfds, client_fd);
}

int readFromClient(int client_fd)
{
    char buf[4096];
    ssize_t bytes_read;

    bytes_read = read(client_fd, buf, sizeof(buf));
    if (bytes_read > 0)
    {
        std::cout << "[client " << client_fd << "] ";
        std::cout.write(buf, bytes_read);
        if (buf[bytes_read - 1] == '\n')
            std::cout.flush();
        return (1);
    }
    if (bytes_read == 0)
    {
        std::cout.flush();
        return (0);
    }
    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        return (1);
    printError("read()");
    return (0);
}

int handleClientData(std::vector<struct pollfd> &pollfds, size_t i)
{
    int client_fd;

    client_fd = pollfds[i].fd;
    if (!readFromClient(client_fd))
    {
        std::cout << "[RaRe Server] client " << client_fd << " disconnected" << std::endl;
        close(client_fd);
        removeClientFromPoll(pollfds, i);
        return (0);
    }
    return (1);
}
