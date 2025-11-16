/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiChild.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <sys/wait.h>
#include <unistd.h>

static void setupChildIO(int inputFd, int outputFd)
{
    dup2(inputFd, STDIN_FILENO);
    dup2(outputFd, STDOUT_FILENO);
    dup2(outputFd, STDERR_FILENO);
    close(inputFd);
    close(outputFd);
}

static void executeCgiChild(int inputFd, int outputFd, const std::string &scriptPath,
                            const std::string &interpreter,
                            const std::string &queryStr, const HttpRequest &req)
{
    char *args[3];
    std::vector<char *> envVec;
    std::vector<std::string> envStrings;
    size_t i;

    setupChildIO(inputFd, outputFd);
    args[0] = const_cast<char *>(interpreter.c_str());
    args[1] = const_cast<char *>(scriptPath.c_str());
    args[2] = NULL;
    buildCgiEnvironment(envStrings, queryStr, req);
    convertEnvToCharArray(envStrings, envVec);
    execve(args[0], args, &envVec[0]);
    i = 0;
    while (i < envVec.size() && envVec[i] != NULL)
    {
        delete[] envVec[i];
        i++;
    }
    return;
}

static pid_t forkCgiChild(int inputPipe[2], int outputPipe[2],
                          const std::string &scriptPath,
                          const std::string &interpreter,
                          const std::string &queryStr,
                          const HttpRequest &req)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        close(inputPipe[0]);
        close(inputPipe[1]);
        close(outputPipe[0]);
        close(outputPipe[1]);
        return (-1);
    }
    if (pid == 0)
    {
        close(inputPipe[1]);
        close(outputPipe[0]);
        executeCgiChild(inputPipe[0], outputPipe[1], scriptPath, interpreter, queryStr, req);
    }
    return (pid);
}

void waitForCgiProcess(pid_t pid, int &status)
{
    int result;
    int retries;

    retries = 0;
    while (retries < 10)
    {
        result = waitpid(pid, &status, WNOHANG);
        if (result == pid)
            return;
        if (result < 0)
            return;
        retries++;
        usleep(10000);
    }
    waitpid(pid, &status, 0);
}

pid_t cgiChildFork(int inputPipe[2], int outputPipe[2],
                   const std::string &scriptPath,
                   const std::string &interpreter,
                   const std::string &queryStr,
                   const HttpRequest &req)
{
    return (forkCgiChild(inputPipe, outputPipe, scriptPath, interpreter, queryStr, req));
}
