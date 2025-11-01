/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientTimeout.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 00:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:59 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <map>

static std::map<int, time_t> &getClientTimestamps(void)
{
    static std::map<int, time_t> clientTimestamps;

    return (clientTimestamps);
}

void updateClientTime(int clientFd)
{
    time_t currentTime;

    currentTime = time(NULL);
    getClientTimestamps()[clientFd] = currentTime;
}

void removeClientTime(int clientFd)
{
    getClientTimestamps().erase(clientFd);
}

void checkClientTimeouts(std::vector<struct pollfd> &pollFds)
{
    time_t currentTime;
    std::map<int, time_t>::iterator it;
    size_t i;

    currentTime = time(NULL);
    i = 0;
    while (i < pollFds.size())
    {
        it = getClientTimestamps().find(pollFds[i].fd);
        if (it != getClientTimestamps().end())
        {
            if ((currentTime - it->second) >= CLIENT_TIMEOUT)
            {
                close(pollFds[i].fd);
                removeClientTime(pollFds[i].fd);
                clearPendingResponse(pollFds[i].fd);
                removeClientFromPoll(pollFds, i);
                continue;
            }
        }
        i++;
    }
}
