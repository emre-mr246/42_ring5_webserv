/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_handler.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/09/13 19:53:49 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int accept_client_connection(int server_fd)
{
    int client_fd;

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (-1);
        print_error("accept()");
        return (-1);
    }
    return (client_fd);
}

void handle_new_connection(std::vector<struct pollfd> &pollfds, int server_fd)
{
    int client_fd;

    client_fd = accept_client_connection(server_fd);
    if (client_fd == -1)
        return;
    std::cout << "[RaRe Server] client " << client_fd << " connected" << std::endl;
    add_client_to_poll(pollfds, client_fd);
}

int read_from_client(int client_fd)
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
    print_error("read()");
    return (0);
}

int handle_client_data(std::vector<struct pollfd> &pollfds, size_t i)
{
    int client_fd;

    client_fd = pollfds[i].fd;
    if (!read_from_client(client_fd))
    {
        std::cout << "[RaRe Server] client " << client_fd << " disconnected" << std::endl;
        close(client_fd);
        remove_client_from_poll(pollfds, i);
        return (0);
    }
    return (1);
}
