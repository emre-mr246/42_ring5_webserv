/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 12:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initPollServer(std::vector<struct pollfd> &pollfds, int server_fd)
{
    struct pollfd server_poll;

    pollfds.clear();
    server_poll.fd = server_fd;
    server_poll.events = POLLIN;
    server_poll.revents = 0;
    pollfds.push_back(server_poll);
}

void addClientToPoll(std::vector<struct pollfd> &pollfds, int client_fd)
{
    struct pollfd client_poll;

    client_poll.fd = client_fd;
    client_poll.events = POLLIN;
    client_poll.revents = 0;
    pollfds.push_back(client_poll);
}

void removeClientFromPoll(std::vector<struct pollfd> &pollfds, size_t index)
{
    if (index < pollfds.size())
        pollfds.erase(pollfds.begin() + index);
}

int waitForEvents(std::vector<struct pollfd> &pollfds)
{
    int result;

    result = poll(pollfds.data(), pollfds.size(), -1);
    if (result == -1)
    {
        printError("poll()");
        return (-1);
    }
    return (result);
}
