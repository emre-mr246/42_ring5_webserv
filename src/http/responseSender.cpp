/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseSender.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 14:45:41 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

int sendResponseToClient(int clientFd, const std::string &response)
{
    ssize_t bytesSent;
    size_t totalSent;
    size_t remaining;

    totalSent = 0;
    remaining = response.length();
    while (totalSent < response.length())
    {
        bytesSent = write(clientFd, response.c_str() + totalSent, remaining);
        if (bytesSent <= 0)
            return (0);
        totalSent = totalSent + bytesSent;
        remaining = remaining - bytesSent;
    }
    return (1);
}
