#pragma once

#include <netinet/in.h>
#include <string>

class Client {
public:
  explicit Client(std::string ip, unsigned short port);

  void Connect();

private:
  int client_socket_fd;
  sockaddr_in server_info{};
};
