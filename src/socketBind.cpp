/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketBind.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:19:27 by emgul            #+#    #+#              */
/*   Updated: 2025/11/04 12:22:15 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <netdb.h>

static void setupAddrInfoHints(struct addrinfo &hints, int family)
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

static int resolveAddressInfo(const std::string &host, const std::string &portStr,
                              struct addrinfo &hints, struct addrinfo **res)
{
    int result;
    std::string errorMsg;
    const char *hostPtr;

    if (host.empty())
        hostPtr = NULL;
    else
        hostPtr = host.c_str();
    result = getaddrinfo(hostPtr, portStr.c_str(), &hints, res);
    if (result != 0)
    {
        errorMsg = "getaddrinfo(): ";
        errorMsg += gai_strerror(result);
        printError(errorMsg);
    }
    return (result);
}

static struct addrinfo *findBindableAddress(int fd, struct addrinfo *addressList)
{
    struct addrinfo *it;
    int bindResult;

    it = addressList;
    while (it)
    {
        bindResult = bind(fd, it->ai_addr, it->ai_addrlen);
        if (bindResult == 0)
            return (it);
        it = it->ai_next;
    }
    printError("bind()");
    return (NULL);
}

static int listenOnSocket(int fd, int backlog, struct addrinfo *boundAddress)
{
    if (!boundAddress)
        return (0);
    if (listen(fd, backlog) == -1)
    {
        printError("listen()");
        return (0);
    }
    return (1);
}

int bindServerSocket(int fd, const std::string &host, int port, int backlog, int family)
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *boundAddress;
    std::ostringstream portStr;

    setupAddrInfoHints(hints, family);
    portStr << port;
    if (resolveAddressInfo(host, portStr.str(), hints, &res) != 0)
        return (-1);
    boundAddress = findBindableAddress(fd, res);
    freeaddrinfo(res);
    if (!listenOnSocket(fd, backlog, boundAddress))
        return (-1);
    return (0);
}
