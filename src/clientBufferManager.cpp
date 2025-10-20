/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientBufferManager.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>

static std::map<int, ClientRequestBuffer> &getClientBuffers(void)
{
    static std::map<int, ClientRequestBuffer> clientBuffers;

    return (clientBuffers);
}

std::string &getClientBuffer(int clientFd)
{
    return (getClientBuffers()[clientFd].buffer);
}

void clearClientBuffer(int clientFd)
{
    getClientBuffers().erase(clientFd);
}

int areHeadersParsed(int clientFd)
{
    ClientRequestBuffer &clientBuffer = getClientBuffers()[clientFd];

    return (clientBuffer.headersParsed);
}

int isRequestComplete(int clientFd)
{
    ClientRequestBuffer &clientBuffer = getClientBuffers()[clientFd];
    size_t headerEnd;
    size_t contentLength;
    size_t terminatorPos;

    headerEnd = findHeaderEnd(clientBuffer.buffer);
    if (headerEnd == std::string::npos)
        return (0);
    if (!clientBuffer.headersParsed)
    {
        clientBuffer.headersParsed = 1;
        contentLength = parseContentLength(clientBuffer.buffer);
        clientBuffer.expectedBodySize = contentLength;
    }
    if (clientBuffer.buffer.find("Transfer-Encoding: chunked") < headerEnd ||
        clientBuffer.buffer.find("transfer-encoding: chunked") < headerEnd)
    {
        terminatorPos = clientBuffer.buffer.find("\r\n0\r\n\r\n", headerEnd);
        if (terminatorPos == std::string::npos)
            terminatorPos = clientBuffer.buffer.find("\n0\n\n", headerEnd);
        return (terminatorPos != std::string::npos);
    }
    return (clientBuffer.buffer.length() - headerEnd >= clientBuffer.expectedBodySize);
}
