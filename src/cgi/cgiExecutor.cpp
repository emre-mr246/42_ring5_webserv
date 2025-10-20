/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

std::string readCgiOutput(int fd)
{
    std::string output;
    char buffer[4096];
    ssize_t bytesRead;

    while (1)
    {
        bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            break;
        buffer[bytesRead] = '\0';
        output.append(buffer, bytesRead);
    }
    return (output);
}

HttpResponse executeCgiScript(const std::string &scriptPath,
                              const std::string &interpreter,
                              const HttpRequest &req, const Config *config)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return (createErrorResponse(500, req, config));
    return (runCgiProcess(pipefd, scriptPath, interpreter, req, config));
}
