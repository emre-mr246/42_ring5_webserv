/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/15 14:55:12 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void processEvents(std::vector<struct pollfd> &pollfds, int server_fd)
{
    size_t i;

    i = 0;
    while (i < pollfds.size())
    {
        if (pollfds[i].revents & POLLIN)
        {
            if (pollfds[i].fd == server_fd)
                handleNewConnection(pollfds, server_fd);
            else
            {
                if (!handleClientData(pollfds, i))
                    i--;
            }
        }
        pollfds[i].revents = 0;
        i++;
    }
}

void eventLoop(int server_fd)
{
    std::vector<struct pollfd> pollfds;

    std::cout << "[INFO] Creating server..." << std::endl;
    initPollServer(pollfds, server_fd);
    std::cout << "[SERVER] Ready to accept connections." << std::endl;
    while (1)
    {
        if (waitForEvents(pollfds) > 0)
            processEvents(pollfds, server_fd);
    }
}
