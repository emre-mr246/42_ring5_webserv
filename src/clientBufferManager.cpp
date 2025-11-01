/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientBufferManager.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>

std::map<int, ClientRequestBuffer> &getClientBuffers(void);

std::string &getClientBuffer(int clientFd)
{
    return (getClientBuffers()[clientFd].buffer);
}

void clearClientBuffer(int clientFd)
{
    std::map<int, ClientRequestBuffer> &buffers = getClientBuffers();
    std::map<int, ClientRequestBuffer>::iterator it = buffers.find(clientFd);
    if (it != buffers.end())
    {
        std::string empty;
        it->second.buffer.swap(empty);
        buffers.erase(it);
    }
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
    std::string buffer = clientBuffer.buffer;

    headerEnd = findHeaderEnd(buffer);
    if (headerEnd == std::string::npos)
        return (0);
    if (!clientBuffer.headersParsed)
    {
        clientBuffer.headersParsed = 1;
        contentLength = parseContentLength(buffer);
        clientBuffer.expectedBodySize = contentLength;
    }
    if (buffer.find("Transfer-Encoding: chunked") != std::string::npos ||
        buffer.find("transfer-encoding: chunked") != std::string::npos)
    {
        terminatorPos = buffer.find("0\r\n\r\n", headerEnd);
        if (terminatorPos == std::string::npos)
            terminatorPos = buffer.find("0\n\n", headerEnd);
        return (terminatorPos != std::string::npos);
    }
    if (clientBuffer.expectedBodySize == 0)
        return (1);
    if (buffer.length() - headerEnd >= clientBuffer.expectedBodySize)
        return (1);
    return (0);
}
