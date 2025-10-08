/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 12:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv)
{
    int serverFd;
    Config serverConfig;

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <configuration file>" << std::endl;
        return (1);
    }
    if (!serverConfig.loadConfig(argv[1]))
        return (1);
    serverFd = createListeningSocket();
    if (serverFd == -1)
        return (1);
    eventLoop(serverFd);
    close(serverFd);
    return (0);
}
