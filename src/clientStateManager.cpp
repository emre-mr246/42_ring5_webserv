/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientStateManager.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:46 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>

std::map<int, ClientRequestBuffer> &getClientBuffers(void)
{
    static std::map<int, ClientRequestBuffer> buffers;

    return (buffers);
}

void cleanupClientBuffers(void)
{
    getClientBuffers().clear();
}

int getClientServerFd(int clientFd)
{
    return (getClientBuffers()[clientFd].serverFd);
}

void setClientServerFd(int clientFd, int serverFd)
{
    getClientBuffers()[clientFd].serverFd = serverFd;
}

void resetClientState(int clientFd)
{
    ClientRequestBuffer *clientBuffer;

    clientBuffer = &getClientBuffers()[clientFd];
    clientBuffer->headersParsed = 0;
    clientBuffer->expectedBodySize = 0;
}
