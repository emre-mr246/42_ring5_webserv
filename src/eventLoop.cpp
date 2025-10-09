/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/10/09 19:48:09 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void processEvents(std::vector<struct pollfd> &pollFds, int serverFd)
{
    size_t i;

    i = 0;
    while (i < pollFds.size())
    {
        if (pollFds[i].fd == serverFd)
        {
            if (pollFds[i].revents & POLLIN)
                handleNewConnection(pollFds, serverFd);
        }
        else
        {
            if (pollFds[i].revents & POLLIN)
            {
                if (!handleClientData(pollFds, i))
                    continue;
            }
            if (i < pollFds.size() && (pollFds[i].revents & POLLOUT))
            {
                if (!handleClientWrite(pollFds, i))
                    continue;
            }
        }
        pollFds[i].revents = 0;
        i++;
    }
}

void eventLoop(int serverFd)
{
    std::vector<struct pollfd> pollFds;

    initPollServer(pollFds, serverFd);
    while (1)
    {
        if (waitForEvents(pollFds) > 0)
            processEvents(pollFds, serverFd);
    }
}
