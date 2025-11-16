/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiSelectLoop.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 13:52:00 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <signal.h>
#include <unistd.h>

static int checkTimeoutElapsed(unsigned long &elapsedMs, pid_t pid)
{
    long delayCounter;

    elapsedMs += 10;
    if (elapsedMs >= (unsigned long)CGI_TIMEOUT)
    {
        kill(pid, SIGTERM);
        delayCounter = 0;
        while (delayCounter < CGI_DELAY_SIGTERM)
            delayCounter++;
        kill(pid, SIGKILL);
        return (1);
    }
}

static void writeToChildProcess(CgiSelectState &state)
{
    size_t remaining;
    ssize_t bytesWritten;

    if (!state.inputDone && state.totalWritten < state.bodyData->length())
    {
        remaining = state.bodyData->length() - state.totalWritten;
        if (remaining > CGI_BUFFER_SIZE)
            remaining = CGI_BUFFER_SIZE;
        bytesWritten = write(state.inputFd, state.bodyData->c_str() + state.totalWritten, remaining);
        if (bytesWritten > 0)
            state.totalWritten += bytesWritten;
        if (state.totalWritten >= state.bodyData->length())
        {
            close(state.inputFd);
            state.inputDone = 1;
        }
    }
}

static int readFromChildProcess(CgiSelectState &state)
{
    char buffer[4096];
    ssize_t bytesRead;

    bytesRead = read(state.outputFd, buffer, sizeof(buffer));
    if (bytesRead > 0 && state.output->length() + bytesRead <= CGI_OUTPUT_MAX_SIZE)
        state.output->append(buffer, bytesRead);
    if (bytesRead == 0 && state.inputDone)
        return (0);
    return (1);
}

static void runCgiLoop(CgiSelectState &state, pid_t pid)
{
    unsigned long elapsedMs;
    unsigned long iterCount;

    elapsedMs = 0;
    iterCount = 0;
    while (1)
    {
        writeToChildProcess(state);
        if (!readFromChildProcess(state))
            break;
        iterCount++;
        if (iterCount >= 10000)
        {
            if (checkTimeoutElapsed(elapsedMs, pid))
                break;
            iterCount = 0;
        }
    }
}

void cgiSelectRun(CgiSelectState &state, pid_t pid)
{
    runCgiLoop(state, pid);
}
