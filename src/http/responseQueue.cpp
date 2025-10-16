/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseQueue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:12 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static std::map<int, PendingResponse> &getPendingResponses(void)
{
    static std::map<int, PendingResponse> pendingResponses;

    return (pendingResponses);
}

void setPendingResponse(int clientFd, const std::string &response)
{
    PendingResponse entry;

    entry.data = response;
    entry.offset = 0;
    getPendingResponses()[clientFd] = entry;
}

int hasPendingResponse(int clientFd)
{
    if (getPendingResponses().find(clientFd) != getPendingResponses().end())
        return (1);
    return (0);
}

int sendPendingResponse(int clientFd)
{
    std::map<int, PendingResponse>::iterator it;

    it = getPendingResponses().find(clientFd);
    if (it == getPendingResponses().end())
        return (2);
    if (!sendResponseToClient(clientFd, it->second.data, it->second.offset))
    {
        getPendingResponses().erase(it);
        return (0);
    }
    if (it->second.offset >= it->second.data.length())
    {
        getPendingResponses().erase(it);
        return (2);
    }
    return (1);
}

void clearPendingResponse(int clientFd)
{
    getPendingResponses().erase(clientFd);
}
