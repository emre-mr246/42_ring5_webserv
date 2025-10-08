/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:12:19 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 11:18:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int acceptClientConnection(int server_fd)
{
    int client_fd;

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (-1);
        printError("accept()");
        return (-1);
    }
    return (client_fd);
}

void handleNewConnection(std::vector<struct pollfd> &pollfds, int server_fd)
{
    int client_fd;

    client_fd = acceptClientConnection(server_fd);
    if (client_fd == -1)
        return;
    addClientToPoll(pollfds, client_fd);
}

static void handleRequest(const HttpRequest &req, int client_fd)
{
    HttpResponse response;
    std::string response_str;

    if (!validateHttpRequest(req))
        response = createErrorResponse(400);
    else if (req.method == "GET")
        response = handleGetRequest(req.uri);
    else
        response = createErrorResponse(405);
    response_str = buildHttpResponse(response);
    sendResponseToClient(client_fd, response_str);
}

static void parseAndHandleRequest(const char *buf, ssize_t bytes_read, int client_fd)
{
    HttpRequest req;

    processRequestData(buf, bytes_read, req);
    printHttpRequest(req);
    handleRequest(req, client_fd);
}

int readFromClient(int client_fd)
{
    char buf[4096];
    ssize_t bytes_read;

    bytes_read = read(client_fd, buf, sizeof(buf));
    if (bytes_read > 0)
    {
        parseAndHandleRequest(buf, bytes_read, client_fd);
        return (1);
    }
    if (bytes_read == 0)
        return (0);
    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        return (1);
    printError("read()");
    return (0);
}

int handleClientData(std::vector<struct pollfd> &pollfds, size_t i)
{
    int client_fd;

    client_fd = pollfds[i].fd;
    if (!readFromClient(client_fd))
    {
        close(client_fd);
        removeClientFromPoll(pollfds, i);
        return (0);
    }
    return (1);
}
