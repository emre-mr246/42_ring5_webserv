/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emgul <emgul@student.42istanbul.com.tr>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:54 by emgul            #+#    #+#              */
/*   Updated: 2025/10/12 17:19:28 by emgul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char **argv) {
  Config serverConfig;
  std::vector<std::pair<std::string, int>> addresses;
  std::vector<int> serverFds;
  size_t i;

  signal(SIGPIPE, SIG_IGN);
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <configuration file>" << std::endl;
    return (1);
  }
  if (!serverConfig.loadConfig(argv[1]))
    return (1);
  gatherAddresses(serverConfig, addresses);
  if (!openServerSockets(addresses, serverFds))
    return (1);
  eventLoop(serverFds);
  i = 0;
  while (i < serverFds.size()) {
    close(serverFds[i]);
    i++;
  }
  return (0);
}
