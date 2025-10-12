/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiTimeout.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 00:00:00 by emgul            #+#    #+#              */
/*   Updated: 2025/10/12 17:19:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>
#include <sys/types.h>
#include <sys/wait.h>

static std::map<pid_t, time_t> cgiProcesses;

void trackCgiProcess(pid_t pid) {
  time_t currentTime;

  currentTime = time(NULL);
  cgiProcesses[pid] = currentTime;
}

void removeCgiProcess(pid_t pid) { cgiProcesses.erase(pid); }

int checkCgiTimeout(pid_t pid) {
  time_t currentTime;
  std::map<pid_t, time_t>::iterator it;
  int status;

  currentTime = time(NULL);
  it = cgiProcesses.find(pid);
  if (it != cgiProcesses.end()) {
    if ((currentTime - it->second) >= CGI_TIMEOUT) {
      kill(pid, SIGKILL);
      waitpid(pid, &status, 0);
      removeCgiProcess(pid);
      return (1);
    }
  }
  return (0);
}
