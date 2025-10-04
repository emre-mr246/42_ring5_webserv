/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   staticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:29:35 by emgul            #+#    #+#              */
/*   Updated: 2025/10/04 21:29:35 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static std::string readFileContent(const std::string &path)
{
    int fd;
    char buf[4096];
    ssize_t bytes;
    std::string content;

    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
        return ("");
    while (1)
    {
        bytes = read(fd, buf, sizeof(buf));
        if (bytes <= 0)
            break;
        content.append(buf, bytes);
    }
    close(fd);
    return (content);
}

static int fileExists(const std::string &path)
{
    struct stat st;

    if (stat(path.c_str(), &st) == -1)
        return (0);
    return (1);
}

HttpResponse serveStaticFile(const std::string &path)
{
    HttpResponse res;

    if (!fileExists(path))
    {
        res.status_code = 404;
        res.body = "<html><body><h1>404 Not Found</h1></body></html>";
        return (res);
    }
    res.status_code = 200;
    res.body = readFileContent(path);
    res.headers["Content-Type"] = "text/html";
    return (res);
}
