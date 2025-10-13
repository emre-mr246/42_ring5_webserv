/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/10/13 05:01:12 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initPollServer(std::vector<struct pollfd> &pollFds,
                    const std::vector<int> &serverFds)
{
    struct pollfd entry;
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

void addClientToPoll(std::vector<struct pollfd> &pollFds, int clientFd)
{
    struct pollfd clientPoll;

    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    pollFds.push_back(clientPoll);
    updateClientTime(clientFd);
}

void removeClientFromPoll(std::vector<struct pollfd> &pollFds, size_t index)
{
    if (index < pollFds.size())
    {
        removeClientTime(pollFds[index].fd);
        pollFds.erase(pollFds.begin() + index);
    }
}

void updateClientEvents(std::vector<struct pollfd> &pollFds, int clientFd,
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

int waitForEvents(std::vector<struct pollfd> &pollFds)
{
    int result;

    while (1)
    {
        result = poll(pollFds.data(), pollFds.size(), -1);
        if (result == -1)
        {
            if (errno == EINTR)
                continue;
            printError("poll()");
            return (-1);
        }
        return (result);
    }
}
