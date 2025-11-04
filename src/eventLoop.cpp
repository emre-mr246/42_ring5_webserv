/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int isServerFd(const std::vector<int> &serverFds, int fd)
{
    size_t i;

    i = 0;
    while (i < serverFds.size())
    {
        if (serverFds[i] == fd)
            return (1);
        i++;
    }
    return (0);
}

static void processEvents(std::vector<struct pollfd> &pollFds,
                          const std::vector<int> &serverFds,
                          const Config *config)
{
    size_t i;

    i = 0;
    while (i < pollFds.size())
    {
        if (isServerFd(serverFds, pollFds[i].fd))
        {
            if (pollFds[i].revents & POLLIN)
                handleNewConnection(pollFds, pollFds[i].fd);
        }
        else
        {
            if ((pollFds[i].revents & POLLIN) && !handleClientData(pollFds, i, config))
                continue;
            if (i < pollFds.size() && (pollFds[i].revents & POLLOUT) &&
                !handleClientWrite(pollFds, i))
                continue;
        }
        pollFds[i].revents = 0;
        i++;
    }
}

void eventLoop(const std::vector<int> &serverFds, const Config *config)
{
    std::vector<struct pollfd> pollFds;

    initPollServer(pollFds, serverFds);
    while (1)
    {
        checkClientTimeouts(pollFds);
        if (waitForEvents(pollFds) > 0)
            processEvents(pollFds, serverFds, config);
    }
}
