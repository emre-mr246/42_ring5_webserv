/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void init_poll_server(std::vector<struct pollfd> &pollfds, int server_fd)
{
    struct pollfd server_poll;

    pollfds.clear();
    server_poll.fd = server_fd;
    server_poll.events = POLLIN;
    server_poll.revents = 0;
    pollfds.push_back(server_poll);
}

void add_client_to_poll(std::vector<struct pollfd> &pollfds, int client_fd)
{
    struct pollfd client_poll;

    client_poll.fd = client_fd;
    client_poll.events = POLLIN;
    client_poll.revents = 0;
    pollfds.push_back(client_poll);
}

void remove_client_from_poll(std::vector<struct pollfd> &pollfds, size_t index)
{
    if (index < pollfds.size())
        pollfds.erase(pollfds.begin() + index);
}

int wait_for_events(std::vector<struct pollfd> &pollfds)
{
    int result;

    result = poll(pollfds.data(), pollfds.size(), -1);
    if (result == -1)
    {
        print_error("poll()");
        return (-1);
    }
    return (result);
}
