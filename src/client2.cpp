/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:56:51 by emgul            #+#    #+#              */
/*   Updated: 2025/10/05 13:39:50 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int handleClientEvents(std::vector<struct pollfd> &pollfds, size_t i)
{
    int client_fd;

    client_fd = pollfds[i].fd;
    if (!readClientData(client_fd))
    {
        std::cout << "[RaRe Server] client " << client_fd << " disconnected" << std::endl;
        close(client_fd);
        pollfds.erase(pollfds.begin() + i);
        return (0);
    }
    return (1);
}
