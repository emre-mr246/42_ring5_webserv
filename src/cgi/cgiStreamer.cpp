/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiStreamer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 23:50:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

static void parseCgiHeaders(const std::string &output, size_t &headerEnd,
                            std::map<std::string, std::string> &headers)
{
    size_t pos;
    size_t lineEnd;
    std::string line;
    size_t colonPos;
    std::string key;
    std::string value;

    pos = 0;
    while (pos < output.length())
    {
        lineEnd = output.find("\r\n", pos);
        if (lineEnd == std::string::npos)
        {
            if (pos == 0)
                return;
            headerEnd = pos;
            return;
        }
        line = output.substr(pos, lineEnd - pos);
        if (line.empty())
        {
            headerEnd = lineEnd + 2;
            return;
        }
        colonPos = line.find(":");
        if (colonPos != std::string::npos)
        {
            key = line.substr(0, colonPos);
            value = line.substr(colonPos + 1);
            if (!value.empty() && value[0] == ' ')
                value = value.substr(1);
            headers[key] = value;
        }
        pos = lineEnd + 2;
    }
    headerEnd = pos;
}

static void extractStatusCode(const std::string &statusLine, int &statusCode,
                              std::string &statusMessage)
{
    size_t spacePos;

    statusCode = 200;
    statusMessage = "OK";
    spacePos = statusLine.find(" ");
    if (spacePos != std::string::npos)
    {
        statusCode = atoi(statusLine.substr(0, spacePos).c_str());
        statusMessage = statusLine.substr(spacePos + 1);
    }
}

static void buildResponseHeaders(std::ostringstream &response,
                                 std::map<std::string, std::string> &headers,
                                 int statusCode, const std::string &statusMessage)
{
    std::map<std::string, std::string>::iterator it;

    response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    it = headers.begin();
    while (it != headers.end())
    {
        if (it->first != "Status")
            response << it->first << ": " << it->second << "\r\n";
        it++;
    }
    if (headers.find("Content-Length") == headers.end())
        response << "Transfer-Encoding: chunked\r\n";
    if (headers.find("Connection") == headers.end())
        response << "Connection: keep-alive\r\n";
    response << "\r\n";
}

static int sendCgiHeaders(int clientFd, const std::string &cgiOutput,
                          size_t &bodyStart)
{
    std::map<std::string, std::string> headers;
    std::ostringstream response;
    int statusCode;
    std::string statusMessage;
    ssize_t bytesSent;
    std::string headerStr;

    parseCgiHeaders(cgiOutput, bodyStart, headers);
    if (headers.find("Status") != headers.end())
        extractStatusCode(headers["Status"], statusCode, statusMessage);
    else
    {
        statusCode = 200;
        statusMessage = "OK";
    }
    buildResponseHeaders(response, headers, statusCode, statusMessage);
    headerStr = response.str();
    bytesSent = write(clientFd, headerStr.c_str(), headerStr.length());
    if (bytesSent <= 0)
        return (0);
    return (1);
}

static int writeToClient(int clientFd, const char *buffer, ssize_t bytesRead)
{
    ssize_t bytesWritten;

    bytesWritten = write(clientFd, buffer, bytesRead);
    if (bytesWritten <= 0)
        return (0);
    return (1);
}

static int pollAndRead(int cgiOutputFd, char *buffer, size_t bufferSize,
                       int *cgiClosed)
{
    struct pollfd polls[1];
    ssize_t bytesRead;

    polls[0].fd = cgiOutputFd;
    polls[0].events = POLLIN;
    if (poll(polls, 1, POLL_TIMEOUT_MSEC) <= 0)
        return (-1);
    if (!(polls[0].revents & POLLIN))
        return (-1);
    bytesRead = read(cgiOutputFd, buffer, bufferSize);
    if (bytesRead > 0)
        return (bytesRead);
    if (bytesRead == 0)
        *cgiClosed = 1;
    return (bytesRead);
}

int streamCgiToClient(int clientFd, int cgiOutputFd, pid_t cgiPid)
{
    char buffer[CGI_BUFFER_SIZE];
    int cgiClosed;
    int status;
    ssize_t bytesRead;

    cgiClosed = 0;
    while (!cgiClosed)
    {
        bytesRead = pollAndRead(cgiOutputFd, buffer, sizeof(buffer), &cgiClosed);
        if (bytesRead > 0)
        {
            if (!writeToClient(clientFd, buffer, bytesRead))
                return (0);
        }
        else if (bytesRead < 0 && !cgiClosed)
            return (0);
    }
    close(cgiOutputFd);
    waitpid(cgiPid, &status, 0);
    return (1);
}
