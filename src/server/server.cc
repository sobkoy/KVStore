#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

void Server::Run() const {
  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket_fd == -1) {
    std::cout << "ERROR: creating socket failed" << std::endl;
    abort();
  }

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

  if (bind(server_socket_fd, reinterpret_cast<sockaddr*>(&server_info), sizeof(server_info)) == -1) {
    std::cerr << "ERROR: bind failed" << std::endl;
    abort();
  }

  if (listen(server_socket_fd, 3) == -1) {
    std::cerr << "ERROR: listen failed" << std::endl;
  } else {
    std::cout << "Listening..." << std::endl;
  }
  int counting_connections = 3;
  while (counting_connections) {
    sockaddr_in client_info{};
    socklen_t client_info_len = sizeof(client_info);
    int client_socket_fd = accept(server_socket_fd, reinterpret_cast<sockaddr*>(&client_info), &client_info_len);
    std::cout << "wait for client...\n" << std::endl;
    if (client_socket_fd == -1) {
      std::cerr << "ERROR: accept client failed" << std::endl;
      continue;
    }

    char buffer[128]{};
    std::size_t buffer_size = sizeof(buffer);
    while (true) {
      std::size_t bytes_read = recv(client_socket_fd, buffer, buffer_size, 0);
      if (bytes_read == -1) {
        std::cerr << "ERROR: recv failed" << std::endl;
        continue;
      }
      if (bytes_read == 0) {
        break;
      }
      std::cout << "Clients input was: " << buffer << std::endl;
      if (buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') {
        strcpy(buffer, "Are you sure you want to exit? [y, N]:");
        if (send(client_socket_fd, buffer, buffer_size, 0) == -1) {
          std::cerr << "ERROR: send failed on exiting" << std::endl;
          continue;
        }
        if (recv(client_socket_fd, buffer, buffer_size, 0) == -1) {
          std::cerr << "ERROR: recv failed on exiting" << std::endl;
          continue;
        }
        if (buffer[0] == 'y') { break; }
      }
      if (send(client_socket_fd, buffer, buffer_size, 0) == -1) {
        std::cerr << "ERROR: echo send failed" << std::endl;
      }
    }
    --counting_connections;
  }

  close(server_socket_fd);

}
