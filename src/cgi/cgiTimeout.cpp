/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiTimeout.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 00:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/11/01 09:59:58 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>
#include <sys/types.h>
#include <sys/wait.h>

static std::map<pid_t, time_t> &getCgiProcesses(void)
{
    static std::map<pid_t, time_t> cgiProcesses;

    return (cgiProcesses);
}

void trackCgiProcess(pid_t pid)
{
    time_t currentTime;

    currentTime = time(NULL);
    getCgiProcesses()[pid] = currentTime;
}

int checkCgiTimeout(pid_t pid)
{
    time_t currentTime;
    std::map<pid_t, time_t>::iterator it;
    int status;

    currentTime = time(NULL);
    it = getCgiProcesses().find(pid);
    if (it != getCgiProcesses().end())
    {
        if ((currentTime - it->second) >= CGI_TIMEOUT)
        {
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            getCgiProcesses().erase(pid);
            return (1);
        }
    }
    return (0);
}
