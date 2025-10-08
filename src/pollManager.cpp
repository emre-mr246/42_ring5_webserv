/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 14:45:40 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void initPollServer(std::vector<struct pollfd> &pollFds, int serverFd)
{
    struct pollfd serverPoll;

    pollFds.clear();
    serverPoll.fd = serverFd;
    serverPoll.events = POLLIN;
    serverPoll.revents = 0;
    pollFds.push_back(serverPoll);
}

void addClientToPoll(std::vector<struct pollfd> &pollFds, int clientFd)
{
    struct pollfd clientPoll;

    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    pollFds.push_back(clientPoll);
}

void removeClientFromPoll(std::vector<struct pollfd> &pollFds, size_t index)
{
    if (index < pollFds.size())
        pollFds.erase(pollFds.begin() + index);
}

int waitForEvents(std::vector<struct pollfd> &pollFds)
{
    int result;

    result = poll(pollFds.data(), pollFds.size(), -1);
    if (result == -1)
    {
        printError("poll()");
        return (-1);
    }
    return (result);
}
