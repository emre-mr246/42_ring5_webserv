/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiExecutor2.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:30:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 18:38:29 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <sstream>
#include <sys/select.h>
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
    exit(1);
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

static void handleCgiInput(int inputFd, const std::string &bodyData, size_t &totalWritten,
                           int &inputDone)
{
    size_t remaining;
    size_t chunkSize;
    ssize_t bytesWritten;

    if (inputDone || totalWritten >= bodyData.length())
        return;
    remaining = bodyData.length() - totalWritten;
    if (remaining > CGI_CHUNK_SIZE)
        chunkSize = CGI_CHUNK_SIZE;
    else
        chunkSize = remaining;
    bytesWritten = write(inputFd, bodyData.c_str() + totalWritten, chunkSize);
    if (bytesWritten > 0)
        totalWritten += bytesWritten;
    if (totalWritten >= bodyData.length())
    {
        close(inputFd);
        inputDone = 1;
    }
}

static int handleCgiOutput(int outputFd, std::string &output)
{
    char buffer[FILE_READ_BUFFER_SIZE];
    ssize_t bytesRead;

    bytesRead = read(outputFd, buffer, sizeof(buffer));
    if (bytesRead < 0 || bytesRead == 0)
        return (0);
    if (output.length() + bytesRead <= CGI_OUTPUT_MAX_SIZE)
        output.append(buffer, bytesRead);
    return (1);
}

static void initializeCgiState(CgiSelectState &state, int inputFd, int outputFd,
                               const std::string &bodyData, std::string &output,
                               pid_t pid)
{
    state.inputFd = inputFd;
    state.outputFd = outputFd;
    state.inputDone = 0;
    state.totalWritten = 0;
    state.bodyData = &bodyData;
    state.output = &output;
    trackCgiProcess(pid);
}

static void setupSelectFds(fd_set &readSet, fd_set &writeSet, CgiSelectState &state,
                           int &maxFd)
{
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_SET(state.outputFd, &readSet);
    maxFd = state.outputFd;
    if (!state.inputDone && state.totalWritten < state.bodyData->length())
    {
        FD_SET(state.inputFd, &writeSet);
        if (state.inputFd > maxFd)
            maxFd = state.inputFd;
    }
}

static int processSelectEvents(fd_set &readSet, fd_set &writeSet, CgiSelectState &state)
{
    if (!state.inputDone && FD_ISSET(state.inputFd, &writeSet))
        handleCgiInput(state.inputFd, *state.bodyData, state.totalWritten, state.inputDone);
    if (FD_ISSET(state.outputFd, &readSet))
    {
        if (!handleCgiOutput(state.outputFd, *state.output))
            return (0);
    }
    return (1);
}

static void waitForCgiProcess(pid_t pid, int &status)
{
    while (waitpid(pid, &status, WNOHANG) == 0)
    {
        usleep(500);
    }
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

static int setupNonblockingPipes(int inputPipe[2], int outputPipe[2])
{
    close(inputPipe[0]);
    close(outputPipe[1]);
    if (fcntl(inputPipe[1], F_SETFL, O_NONBLOCK) == -1)
        return (0);
    if (fcntl(outputPipe[0], F_SETFL, O_NONBLOCK) == -1)
        return (0);
    return (1);
}

static void runCgiSelectLoop(CgiSelectState &state, pid_t pid)
{
    fd_set readSet;
    fd_set writeSet;
    int maxFd;
    struct timeval timeout;
    int selectResult;
    int timeoutCount;

    timeoutCount = 0;
    while (1)
    {
        setupSelectFds(readSet, writeSet, state, maxFd);
        timeout.tv_sec = 0;
        timeout.tv_usec = SELECT_TIMEOUT_USEC;
        selectResult = select(maxFd + 1, &readSet, &writeSet, NULL, &timeout);
        if (selectResult < 0 || (selectResult == 0 && state.inputDone))
            break;
        if (selectResult == 0)
        {
            timeoutCount++;
            if (timeoutCount > 60000)
                break;
        }
        else
            timeoutCount = 0;
        if (!processSelectEvents(readSet, writeSet, state))
            break;
        if (checkCgiTimeout(pid))
            break;
    }
}

static std::string handleCgiWithSelect(int inputFd, int outputFd, pid_t pid,
                                       const std::string &bodyData, int &status)
{
    CgiSelectState state;
    std::string output;

    initializeCgiState(state, inputFd, outputFd, bodyData, output, pid);
    runCgiSelectLoop(state, pid);
    close(outputFd);
    waitForCgiProcess(pid, status);
    return (output);
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
    pid = forkCgiChild(inputPipe, outputPipe, scriptPath, interpreter, queryStr, req);
    if (pid == -1)
        return (createErrorResponse(500, req, config));
    if (!setupNonblockingPipes(inputPipe, outputPipe))
        return (createErrorResponse(500, req, config));
    output = handleCgiWithSelect(inputPipe[1], outputPipe[0], pid, req.body, status);
    return (createCgiResponse(status, output, req, config));
}
