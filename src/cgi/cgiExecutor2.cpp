/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/20 19:54:03 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

static void executeCgiChild(int pipefd, const std::string &scriptPath,
                            const std::string &interpreter,
                            const std::string &queryStr)
{
    char *args[3];
    char *env[2];

    dup2(pipefd, STDOUT_FILENO);
    dup2(pipefd, STDERR_FILENO);
    close(pipefd);
    args[0] = const_cast<char *>(interpreter.c_str());
    args[1] = const_cast<char *>(scriptPath.c_str());
    args[2] = NULL;
    env[0] = createEnvString(queryStr);
    env[1] = NULL;
    execve(args[0], args, env);
    exit(1);
}

static std::string collectCgiOutput(int pipefd, pid_t pid, int &status)
{
    std::string output;
    pid_t result;

    trackCgiProcess(pid);
    while (1)
    {
        result = waitpid(pid, &status, WNOHANG);
        if (result != 0)
            break;
        if (checkCgiTimeout(pid))
        {
            close(pipefd);
            status = -1;
            return ("");
        }
        usleep(100000);
    }
    output = readCgiOutput(pipefd);
    close(pipefd);
    return (output);
}

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

HttpResponse runCgiProcess(int pipefd[2], const std::string &scriptPath,
                           const std::string &interpreter, const HttpRequest &req,
                           const Config *config)
{
    pid_t pid;
    int status;
    std::string output;
    std::string queryStr;

    queryStr = extractQueryString(req.uri);
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (createErrorResponse(500, req, config));
    }
    if (pid == 0)
    {
        close(pipefd[0]);
        executeCgiChild(pipefd[1], scriptPath, interpreter, queryStr);
    }
    close(pipefd[1]);
    output = collectCgiOutput(pipefd[0], pid, status);
    return (createCgiResponse(status, output, req, config));
}
