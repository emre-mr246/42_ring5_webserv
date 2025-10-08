/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 13:39:50 by emgul            #+#    #+#              */
/*   Updated: 2025/10/08 11:18:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sys/stat.h>

static int checkFileExists(const std::string &path)
{
    struct stat file_info;

    if (stat(path.c_str(), &file_info) == -1)
        return (0);
    if (S_ISDIR(file_info.st_mode))
        return (0);
    return (1);
}

static std::string readFileContent(int fd)
{
    std::string content;
    char buf[4096];
    ssize_t bytes_read;

    while (1)
    {
        bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read <= 0)
            break;
        content.append(buf, bytes_read);
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
