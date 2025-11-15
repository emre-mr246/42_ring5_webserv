/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiIO.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/15 19:20:47 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <fcntl.h>
#include <unistd.h>

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
                               const std::string &bodyData, std::string &output)
{
    state.inputFd = inputFd;
    state.outputFd = outputFd;
    state.inputDone = 0;
    state.totalWritten = 0;
    state.bodyData = &bodyData;
    state.output = &output;
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

int cgiIOSetupPipes(int inputPipe[2], int outputPipe[2])
{
    return (setupNonblockingPipes(inputPipe, outputPipe));
}

int cgiIOProcessSelectEvents(fd_set &readSet, fd_set &writeSet, CgiSelectState &state)
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

std::string cgiIOHandleWithSelect(int inputFd, int outputFd, pid_t pid,
                                  const std::string &bodyData, int &status)
{
    CgiSelectState state;
    std::string output;

    initializeCgiState(state, inputFd, outputFd, bodyData, output);
    cgiSelectRun(state, pid);
    close(outputFd);
    waitForCgiProcess(pid, status);
    return (output);
}
