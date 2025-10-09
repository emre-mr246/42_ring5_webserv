/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/09 19:48:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int sendResponseToClient(int clientFd, std::string &response, size_t &offset)
{
    ssize_t bytesSent;
    size_t remaining;

    while (offset < response.length())
    {
        remaining = response.length() - offset;
        bytesSent = write(clientFd, response.c_str() + offset, remaining);
        if (bytesSent <= 0)
            return (0);
        offset = offset + bytesSent;
    }
    return (1);
}
