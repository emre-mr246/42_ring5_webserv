/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:04:46 by emgul            #+#    #+#              */
/*   Updated: 2025/10/14 15:25:06 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printError(const std::string &message)
{
    std::ostringstream oss;

    oss << "[ERROR] " << message << " (" << errno << ") \n";
    std::cerr << oss.str() << std::endl;
}
