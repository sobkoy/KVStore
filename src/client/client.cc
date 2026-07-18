#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

Client::Client(std::string ip, unsigned short port) {
  client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket_fd == -1) {
    std::cerr << "ERROR: creating socket failed" << std::endl;
    abort();
  }

  server_info.sin_family = AF_INET;
  server_info.sin_port = htons(port);

  if (inet_pton(AF_INET, ip.c_str(), &server_info.sin_addr) != 1) {
    std::cerr << "ERROR: Invalid IP address" << std::endl;
    abort();
  }
}

void Client::Connect() {
  if (connect(client_socket_fd, reinterpret_cast<sockaddr*>(&server_info), sizeof(server_info)) == -1) {
    std::cerr << "ERROR: connecting to server failed" << std::endl;
    abort();
  }
  char buffer[128]{};
  long int buffer_size = sizeof(buffer);

  while (true) {
    std::cout << "Enter your text: ";
    std::cin.getline(buffer, buffer_size);
    std::size_t bytes_sent = send(client_socket_fd, buffer, buffer_size, 0);
    if (bytes_sent == -1) {
      std::cerr << "ERROR: sending data failed" << std::endl;
      continue;
    }

    if (recv(client_socket_fd, buffer, buffer_size, 0) == -1) {
      std::cerr << "ERROR: receiving echoed data failed" << std::endl;
      continue;
    }
    std::cout << buffer << std::endl;
  }

}