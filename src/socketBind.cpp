/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketBind.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:19:27 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <netdb.h>

int getAddressInfo(const std::string &host, int port, addrinfo **res)
{
    addrinfo hints;
    std::ostringstream portStr;
    const char *hostPtr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_addr = NULL;
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;
    hints.ai_addrlen = 0;
    portStr << port;
    if (host.empty())
        hostPtr = NULL;
    else
        hostPtr = host.c_str();
    if (getaddrinfo(hostPtr, portStr.str().c_str(), &hints, res) != 0)
        return (-1);
    return (0);
}

int tryBindToAddresses(int fd, addrinfo *res)
{
    addrinfo *it;

    it = res;
    while (it)
    {
        if (bind(fd, it->ai_addr, it->ai_addrlen) == 0)
            return (0);
        it = it->ai_next;
    }
    printError("bind()");
    return (-1);
}

int startListening(int fd, int backlog)
{
    if (listen(fd, backlog) == -1)
    {
        printError("listen()");
        return (-1);
    }
    return (0);
}

int bindServerSocket(int fd, const std::string &host, int port, int backlog)
{
    addrinfo *res;

    if (getAddressInfo(host, port, &res) == -1)
        return (-1);
    if (tryBindToAddresses(fd, res) == -1)
    {
        freeaddrinfo(res);
        return (-1);
    }
    if (startListening(fd, backlog) == -1)
    {
        freeaddrinfo(res);
        return (-1);
    }
    freeaddrinfo(res);
    return (0);
}
