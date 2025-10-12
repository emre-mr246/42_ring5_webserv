/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseQueue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:48:08 by emgul            #+#    #+#              */
/*   Updated: 2025/10/12 17:19:27 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "webserv.hpp"

struct PendingResponse {
  std::string data;
  size_t offset;
};

static std::map<int, PendingResponse> gPendingResponses;

void setPendingResponse(int clientFd, const std::string &response) {
  PendingResponse entry;

  entry.data = response;
  entry.offset = 0;
  gPendingResponses[clientFd] = entry;
}

int hasPendingResponse(int clientFd) {
  if (gPendingResponses.find(clientFd) != gPendingResponses.end())
    return (1);
  return (0);
}

int sendPendingResponse(int clientFd) {
  std::map<int, PendingResponse>::iterator it;

  it = gPendingResponses.find(clientFd);
  if (it == gPendingResponses.end())
    return (2);
  if (!sendResponseToClient(clientFd, it->second.data, it->second.offset)) {
    gPendingResponses.erase(it);
    return (0);
  }
  if (it->second.offset >= it->second.data.length()) {
    gPendingResponses.erase(it);
    return (2);
  }
  return (1);
}

void clearPendingResponse(int clientFd) { gPendingResponses.erase(clientFd); }
