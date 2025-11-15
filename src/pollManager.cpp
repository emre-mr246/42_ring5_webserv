/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initPollServer(std::vector<pollfd> &pollFds, const std::vector<int> &serverFds)
{
    pollfd entry;
    size_t i;

    pollFds.clear();
    i = 0;
    while (i < serverFds.size())
    {
        entry.fd = serverFds[i];
        entry.events = POLLIN;
        entry.revents = 0;
        pollFds.push_back(entry);
        i++;
    }
}

void addClientToPoll(std::vector<pollfd> &pollFds, int clientFd)
{
    pollfd clientPoll;

    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    pollFds.push_back(clientPoll);
}

void removeClientFromPoll(std::vector<pollfd> &pollFds, size_t index)
{
    if (index < pollFds.size())
    {
        pollFds.erase(pollFds.begin() + index);
    }
}

void updateClientEvents(std::vector<pollfd> &pollFds, int clientFd,
                        short events)
{
    size_t i;

    i = 0;
    while (i < pollFds.size())
    {
        if (pollFds[i].fd == clientFd)
        {
            pollFds[i].events = events;
            return;
        }
        i++;
    }
}

int waitForEvents(std::vector<pollfd> &pollFds)
{
    int result;

    while (1)
    {
        result = poll(pollFds.data(), pollFds.size(), POLL_TIMEOUT_MSEC);
        if (result != -1)
            return (result);
    }
}
