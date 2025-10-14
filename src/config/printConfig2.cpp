/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/10/14 15:25:07 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printAcceptedMethods(const std::vector<std::string> &acceptedMethods)
{
    std::vector<std::string>::const_iterator it;

    if (acceptedMethods.empty())
    {
        std::cout << "[CONFIG]   Accepted methods: (none specified)" << std::endl;
        return;
    }
    std::cout << "[CONFIG]   Accepted methods: ";
    it = acceptedMethods.begin();
    while (it != acceptedMethods.end())
    {
        std::cout << *it;
        it++;
        if (it != acceptedMethods.end())
            std::cout << ", ";
    }
    std::cout << std::endl;
}
