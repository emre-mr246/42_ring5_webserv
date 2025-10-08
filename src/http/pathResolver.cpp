/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathResolver.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 11:18:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

std::string resolveFilePath(const std::string &uri)
{
    std::string root;
    std::string path;

    root = "./www";
    if (uri == "/")
        path = root + "/index.html";
    else
        path = root + uri;
    return (path);
}
