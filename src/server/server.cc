#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

void Server::Run() const {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  in_addr str_to_net_addr_struct{};
  if (inet_pton(AF_INET, "0.0.0.0", &str_to_net_addr_struct) <= 0) {
    std::cerr << "ERROR: inet_pton failed" << std::endl;
    abort();
  }

  sockaddr_in server_info{
    .sin_family = AF_INET,
    .sin_port = htons(port_),
    .sin_addr = str_to_net_addr_struct
  };

  // next step is to bind socket
}
