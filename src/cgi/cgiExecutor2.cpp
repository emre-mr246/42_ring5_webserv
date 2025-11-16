/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <sstream>
#include <sys/wait.h>

static HttpResponse createCgiResponse(int status, const std::string &output,
                                      const HttpRequest &req,
                                      const Config *config)
{
    HttpResponse response;

    if (status == -1)
        return (createErrorResponse(504, req, config));
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        response = parseCgiOutput(output);
        if (response.statusCode >= 400)
            return (createErrorResponse(response.statusCode, req, config));
        return (response);
    }
    return (createErrorResponse(500, req, config));
}

HttpResponse runCgiProcess(int inputPipe[2], int outputPipe[2], const std::string &scriptPath,
                           const std::string &interpreter, const HttpRequest &req,
                           const Config *config)
{
    pid_t pid;
    int status;
    std::string output;
    std::string queryStr;

    queryStr = extractQueryString(req.uri);
    pid = cgiChildFork(inputPipe, outputPipe, scriptPath, interpreter, queryStr, req);
    if (pid == -1)
        return (createErrorResponse(500, req, config));
    if (!cgiIOSetupPipes(inputPipe, outputPipe))
        return (createErrorResponse(500, req, config));
    output = cgiIOHandleWithSelect(inputPipe[1], outputPipe[0], pid, req.body, status);
    return (createCgiResponse(status, output, req, config));
}
