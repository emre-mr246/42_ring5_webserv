/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler2.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:09 by emgul            #+#    #+#              */
/*   Updated: 2025/10/09 19:48:09 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int handleClientData(std::vector<struct pollfd> &pollFds, size_t i)
{
    int clientFd;

    clientFd = pollFds[i].fd;
    if (!readFromClient(clientFd, pollFds))
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    return (1);
}

int handleClientWrite(std::vector<struct pollfd> &pollFds, size_t i)
{
    int clientFd;
    int status;

    clientFd = pollFds[i].fd;
    if (!hasPendingResponse(clientFd))
    {
        updateClientEvents(pollFds, clientFd, POLLIN);
        return (1);
    }
    status = sendPendingResponse(clientFd);
    if (status == 0)
    {
        close(clientFd);
        clearPendingResponse(clientFd);
        removeClientFromPoll(pollFds, i);
        return (0);
    }
    if (status == 2)
        updateClientEvents(pollFds, clientFd, POLLIN);
    return (1);
}
