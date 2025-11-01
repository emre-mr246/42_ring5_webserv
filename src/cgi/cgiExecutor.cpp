/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 08:12:21 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:29 by emgul            ###   ########.fr       */
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
    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe) == -1)
        return (createErrorResponse(500, req, config));
    if (pipe(outputPipe) == -1)
    {
        close(inputPipe[0]);
        close(inputPipe[1]);
        return (createErrorResponse(500, req, config));
    }
    return (runCgiProcess(inputPipe, outputPipe, scriptPath, interpreter, req, config));
}
