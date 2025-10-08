/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 11:18:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int sendResponseToClient(int client_fd, const std::string &response)
{
    ssize_t bytes_sent;
    size_t total_sent;
    size_t remaining;

    total_sent = 0;
    remaining = response.length();
    while (total_sent < response.length())
    {
        bytes_sent = write(client_fd, response.c_str() + total_sent, remaining);
        if (bytes_sent <= 0)
            return (0);
        total_sent = total_sent + bytes_sent;
        remaining = remaining - bytes_sent;
    }
    return (1);
}
