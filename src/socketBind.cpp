#include "webserv.hpp"
#include <netdb.h>

int bindServerSocket(int fd, const std::string &host, int port, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *it;
    std::ostringstream portStr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    portStr << port;
    if (getaddrinfo(host.empty() ? NULL : host.c_str(), portStr.str().c_str(), &hints, &res) != 0)
        return (-1);
    it = res;
    while (it)
    {
        if (bind(fd, it->ai_addr, it->ai_addrlen) == 0)
            break;
        it = it->ai_next;
    }
    if (!it)
        printError("bind()");
    if (it && listen(fd, backlog) == -1)
    {
        printError("listen()");
        it = NULL;
    }
    freeaddrinfo(res);
    if (!it)
        return (-1);
    return (0);
}
