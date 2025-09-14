/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:15:47 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void process_events(std::vector<struct pollfd> &pollfds, int server_fd)
{
    size_t i;

    i = 0;
    while (i < pollfds.size())
    {
        if (pollfds[i].revents & POLLIN)
        {
            if (pollfds[i].fd == server_fd)
                handle_new_connection(pollfds, server_fd);
            else
            {
                if (!handle_client_data(pollfds, i))
                    i--;
            }
        }
        pollfds[i].revents = 0;
        i++;
    }
}

void event_loop(int server_fd)
{
    std::vector<struct pollfd> pollfds;

    init_poll_server(pollfds, server_fd);
    while (1)
    {
        if (wait_for_events(pollfds) > 0)
            process_events(pollfds, server_fd);
    }
}
