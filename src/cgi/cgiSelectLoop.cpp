/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiSelectLoop.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/16 12:49:45 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>

static int handleSelectTimeout(int &timeoutCount, int &totalMs, pid_t pid)
{
    timeoutCount++;
    totalMs += 1;
    if (totalMs > CGI_TIMEOUT * 1000)
    {
        kill(pid, SIGTERM);
        usleep(50000);
        kill(pid, SIGKILL);
        return (1);
    }
    if (timeoutCount > 100)
        return (1);
    return (0);
}

static void resetTimeoutIfDataReceived(int selectResult, int &timeoutCount)
{
    if (selectResult > 0)
        timeoutCount = 0;
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
    return (cgiIOProcessSelectEvents(readSet, writeSet, state));
}

static void runCgiSelectLoop(CgiSelectState &state, pid_t pid)
{
    fd_set readSet;
    fd_set writeSet;
    int maxFd;
    timeval timeout;
    int selectResult;
    int timeoutCount;
    int totalMs;

    timeoutCount = 0;
    totalMs = 0;
    while (1)
    {
        setupSelectFds(readSet, writeSet, state, maxFd);
        timeout.tv_sec = 0;
        timeout.tv_usec = SELECT_TIMEOUT_USEC;
        selectResult = select(maxFd + 1, &readSet, &writeSet, NULL, &timeout);
        if (selectResult < 0)
            break;
        if (selectResult == 0)
        {
            if (handleSelectTimeout(timeoutCount, totalMs, pid))
                break;
        }
        else
        {
            resetTimeoutIfDataReceived(selectResult, timeoutCount);
            if (!processSelectEvents(readSet, writeSet, state))
                break;
        }
    }
}

void cgiSelectRun(CgiSelectState &state, pid_t pid)
{
    runCgiSelectLoop(state, pid);
}
