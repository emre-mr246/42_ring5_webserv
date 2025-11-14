/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/11/14 03:22:31 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sys/stat.h>
#include <unistd.h>

static int checkFileExists(const std::string &path)
{
    stat_t fileInfo;

    if (stat(path.c_str(), &fileInfo) == -1)
        return (0);
    if (S_ISDIR(fileInfo.st_mode))
        return (0);
    return (1);
}

static std::string readFileContent(int fd)
{
    std::string content;
    char buf[4096];
    ssize_t bytesRead;

    while (1)
    {
        bytesRead = read(fd, buf, sizeof(buf));
        if (bytesRead <= 0)
            break;
        content.append(buf, bytesRead);
    }
    return (content);
}

int readFile(const std::string &path, std::string &content)
{
    int fd;

    if (!checkFileExists(path))
        return (0);
    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
        return (0);
    content = readFileContent(fd);
    close(fd);
    return (1);
}
