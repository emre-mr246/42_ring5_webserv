/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/09/14 12:04:10 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv)
{
    int server_fd;

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <configuration file>" << std::endl;
        return (1);
    }
    server_fd = create_listening_socket();
    if (server_fd == -1)
        return (1);
    event_loop(server_fd);
    close(server_fd);
    return (0);
}
