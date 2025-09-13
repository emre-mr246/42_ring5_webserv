/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/09/13 08:21:18 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void event_loop(int server_fd)
{
    int client_fd;

    while (1)
    {
        client_fd = accept_client(server_fd);
        if (client_fd == -1)
        {
            usleep(100000);
            continue;
        }
        std::cout << "[RaRe Server] client connected: fd=" << client_fd << std::endl;
        read_and_print_client(client_fd);
        std::cout << "[RaRe Server] client disconnected: fd=" << client_fd << std::endl;
        close(client_fd);
    }
}

int main()
{
    int server_fd;

    server_fd = create_listening_socket();
    if (server_fd == -1)
        return (1);
    event_loop(server_fd);
    close(server_fd);
    return (0);
}
