/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:05:30 by emgul            #+#    #+#              */
/*   Updated: 2025/09/17 14:53:08 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int isComment(const std::string &line)
{
    size_t i;

    i = 0;
    while (i < line.length())
    {
        if (!isspace(line[i]))
        {
            if (line[i] == '#')
                return (1);
            else
                return (0);
        }
        i++;
    }
    return (0);
}

bool isServerBlock(const std::string &line)
{
    std::string trimmed;

    trimmed = strtrim(line);
    return (trimmed == "server" || trimmed == "server {");
}

bool isLocationBlock(const std::string &line)
{
    std::string trimmed;

    trimmed = strtrim(line);
    return (trimmed.find("location") == 0);
}
