/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileWriter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/16 12:56:13 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

static int writeFileContent(int fd, const std::string &content)
{
    size_t totalWritten;
    size_t bytesToWrite;
    ssize_t bytesWritten;

    totalWritten = 0;
    while (totalWritten < content.length())
    {
        bytesToWrite = content.length() - totalWritten;
        bytesWritten = write(fd, content.c_str() + totalWritten, bytesToWrite);
        if (bytesWritten <= 0)
            return (0);
        totalWritten = totalWritten + bytesWritten;
    }
    return (1);
}

int writeFile(const std::string &path, const std::string &content)
{
    int fd;
    int writeResult;

    fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (0);
    writeResult = writeFileContent(fd, content);
    close(fd);
    return (writeResult);
}
