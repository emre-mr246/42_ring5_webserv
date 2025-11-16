/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseQueues.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::map<int, PendingResponse> &getPendingResponses(void)
{
    static std::map<int, PendingResponse> pendingResponses;

    return (pendingResponses);
}

std::map<int, std::vector<ResponseQueueEntry> > &getResponseQueues(void)
{
    static std::map<int, std::vector<ResponseQueueEntry> > responseQueues;

    return (responseQueues);
}

void cleanupResponseQueues(void)
{
    getPendingResponses().clear();
    getResponseQueues().clear();
}

static int loadNextQueuedResponse(int clientFd, std::vector<ResponseQueueEntry> &queue)
{
    PendingResponse pending;

    if (queue.empty())
        return (0);
    pending.data = queue[0].data;
    pending.offset = 0;
    pending.shouldClose = queue[0].shouldClose;
    getPendingResponses()[clientFd] = pending;
    queue.erase(queue.begin());
    return (1);
}

static int handleResponseSent(int clientFd, std::map<int, PendingResponse>::iterator it,
                              std::vector<ResponseQueueEntry> &queue)
{
    int loadResult;

    if (it->second.offset >= it->second.data.length())
    {
        getPendingResponses().erase(it);
        loadResult = loadNextQueuedResponse(clientFd, queue);
        if (loadResult)
            return (1);
        else
            return (2);
    }
    return (1);
}

int sendPendingResponse(int clientFd)
{
    std::map<int, PendingResponse>::iterator it;
    std::vector<ResponseQueueEntry> &queue = getResponseQueues()[clientFd];

    it = getPendingResponses().find(clientFd);
    if (it == getPendingResponses().end())
        return (2);
    if (!sendResponseToClient(clientFd, it->second.data, it->second.offset))
    {
        getPendingResponses().erase(it);
        return (0);
    }
    return (handleResponseSent(clientFd, it, queue));
}
