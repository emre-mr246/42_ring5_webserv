/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionValidationHelper.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int handleBufferOverflow(int clientFd, std::vector<pollfd> &pollFds)
{
    HttpResponse response;
    std::string responseStr;

    response = createErrorResponse(413);
    response.headers["Connection"] = "close";
    responseStr = buildHttpResponse(response);
    setPendingResponse(clientFd, responseStr, true);
    updateClientEvents(pollFds, clientFd, POLLIN | POLLOUT);
    clearClientBuffer(clientFd);
    return (1);
}

static size_t getChunkedTerminatorPosition(const std::string &buffer, size_t headerEnd)
{
    size_t terminatorPos;

    terminatorPos = buffer.find("0\r\n\r\n", headerEnd);
    if (terminatorPos != std::string::npos)
        return (terminatorPos + 5);
    terminatorPos = buffer.find("0\n\n", headerEnd);
    if (terminatorPos != std::string::npos)
        return (terminatorPos + 3);
    return (0);
}

static size_t calculateRequestBytes(const std::string &buffer)
{
    size_t headerEnd;
    size_t contentLength;
    size_t totalBytes;

    headerEnd = findHeaderEnd(buffer);
    if (headerEnd == std::string::npos)
        return (0);
    if (buffer.find("Transfer-Encoding: chunked") != std::string::npos ||
        buffer.find("transfer-encoding: chunked") != std::string::npos)
        return (getChunkedTerminatorPosition(buffer, headerEnd));
    contentLength = parseContentLength(buffer);
    totalBytes = headerEnd + contentLength;
    if (totalBytes > buffer.length())
        return (0);
    return (totalBytes);
}

static int processCompleteRequest(int clientFd, std::vector<pollfd> &pollFds,
                                  const Config *config, std::string &clientBuffer)
{
    size_t processedBytes;

    if (clientBuffer.length() > MAX_REQUEST_BUFFER)
        return (handleBufferOverflow(clientFd, pollFds));
    processedBytes = calculateRequestBytes(clientBuffer);
    if (processedBytes > 0 && processedBytes <= clientBuffer.length())
    {
        parseAndHandleRequest(clientBuffer.c_str(), processedBytes,
                              clientFd, pollFds, config);
        clientBuffer.erase(0, processedBytes);
        resetClientState(clientFd);
    }
    else
        clearClientBuffer(clientFd);
    return (1);
}

int processNewData(int clientFd, std::vector<pollfd> &pollFds, const Config *config, int wasHeadersParsed)
{
    std::string *clientBuffer;
    int shouldContinue;
    int isComplete;

    clientBuffer = &getClientBuffer(clientFd);
    if (clientBuffer->length() > MAX_REQUEST_BUFFER)
        return (handleBufferOverflow(clientFd, pollFds));
    if (!wasHeadersParsed && areHeadersParsed(clientFd))
    {
        checkHeadersForBodySize(clientFd, pollFds, config, &shouldContinue);
        if (!shouldContinue)
        {
            clearClientBuffer(clientFd);
            return (1);
        }
    }
    isComplete = isRequestComplete(clientFd);
    if (isComplete)
        return (processCompleteRequest(clientFd, pollFds, config, *clientBuffer));
    return (1);
}
