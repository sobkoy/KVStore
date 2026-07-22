#include "server.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <cerrno>

#define MAX_EVENTS 10
#define WAIT_TIMEOUT 20'000


int CreateAndBindServerSocket(unsigned short port) {
  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket_fd == -1) {
    std::cerr << "ERROR: creating socket failed --> " << std::strerror(errno) << std::endl;
    abort();
  }

  int opt = 1;
  setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  in_addr str_to_net_addr_struct{};
  if (inet_pton(AF_INET, "0.0.0.0", &str_to_net_addr_struct) <= 0) {
    std::cerr << "ERROR: inet_pton failed --> " << std::strerror(errno) << std::endl;
    abort();
  }

  sockaddr_in server_info{
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr = str_to_net_addr_struct
  };

  if (bind(server_socket_fd, reinterpret_cast<sockaddr*>(&server_info), sizeof(server_info)) == -1) {
    std::cerr << "ERROR: bind failed --> " << std::strerror(errno) << std::endl;
    abort();
  }

  if (listen(server_socket_fd, 3) == -1) {
    std::cerr << "ERROR: listen failed --> " << std::strerror(errno) << std::endl;
  }

  return server_socket_fd;
}

void SetNonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void HandleEvents(epoll_event* events, int nfds) {

}


void Server::Run() const {
  int server_fd = CreateAndBindServerSocket(port_);
  SetNonblocking(server_fd);

  int epoll_fd = epoll_create1(0);

  epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = server_fd;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

  epoll_event events[MAX_EVENTS];

  while (true) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, WAIT_TIMEOUT);
    if (nfds == 0) {
      std::cerr << "ERROR: epoll_wait errno --> " << std::strerror(errno) << std::endl;
      break;
    }

    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == server_fd) {
        while (true) {
          sockaddr_in client_info{};
          socklen_t client_info_len = sizeof(client_info);
          int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_info), &client_info_len);

          if (client_fd == -1) {
            if (errno == EAGAIN) {
              std::cerr << "ERROR: accept client failed--> " << std::strerror(errno) << std::endl;
              break;
            }
          }

          SetNonblocking(client_fd);
          event.events = EPOLLIN | EPOLLET | EPOLLHUP;
          event.data.fd = client_fd;
          epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
          std::cout << "Client connected fd " << client_fd << std::endl;
        }
      } else {
        HandleEvents(events, nfds);
      }
    }
  }

  close(server_fd);
  close(epoll_fd);





  int counting_connections = 3;
  while (counting_connections) {
    sockaddr_in client_info{};
    socklen_t client_info_len = sizeof(client_info);
    std::cout << "Wait for client...\n" << std::endl;
    int client_socket_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_info), &client_info_len);
    if (client_socket_fd == -1) {
      std::cerr << "ERROR: accept client failed" << std::endl;
      continue;
    }

    char buffer[128]{};
    std::size_t buffer_size = sizeof(buffer);
    while (true) {
      ssize_t bytes_read = recv(client_socket_fd, buffer, buffer_size, 0);
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

  close(server_fd);
}
