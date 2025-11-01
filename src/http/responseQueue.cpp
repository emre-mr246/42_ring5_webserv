/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseQueue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::map<int, PendingResponse> &getPendingResponses(void);
std::map<int, std::vector<ResponseQueueEntry> > &getResponseQueues(void);

void setPendingResponse(int clientFd, const std::string &response, bool shouldClose)
{
    ResponseQueueEntry entry;
    PendingResponse pending;
    std::vector<ResponseQueueEntry> &queue = getResponseQueues()[clientFd];

    entry.data = response;
    entry.shouldClose = shouldClose;
    getResponseQueues()[clientFd].push_back(entry);
    if (!hasPendingResponse(clientFd))
    {
        if (!queue.empty())
        {
            pending.data = queue[0].data;
            pending.offset = 0;
            pending.shouldClose = queue[0].shouldClose;
            getPendingResponses()[clientFd] = pending;
            queue.erase(queue.begin());
        }
    }
}

int hasPendingResponse(int clientFd)
{
    if (getPendingResponses().find(clientFd) != getPendingResponses().end())
        return (1);
    return (0);
}

void clearPendingResponse(int clientFd)
{
    getPendingResponses().erase(clientFd);
    getResponseQueues().erase(clientFd);
}

bool shouldCloseConnection(int clientFd)
{
    std::map<int, PendingResponse>::iterator it;

    it = getPendingResponses().find(clientFd);
    if (it == getPendingResponses().end())
        return (false);
    return (it->second.shouldClose);
}
