/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:04:46 by emgul            #+#    #+#              */
/*   Updated: 2025/09/13 08:21:18 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

void log_error(const std::string &context)
{
    int e;

    e = errno;
    std::cerr << "[RaRe Server][ERROR] " << context << " | errno=" << e << " (" << std::strerror(e) << ")" << std::endl;
}