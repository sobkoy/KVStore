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
  for (int i{}; i < nfds; ++i) {
    int fd = events[i].data.fd;

    if (events[i].events & EPOLLHUP) {
      std::cout << "Disconnected fd" << fd << std::endl;
      close(fd);
    } else if (events[i].events & EPOLLIN) {
      char buffer[128]{};
      ssize_t count;
      while ((count = recv(fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[count] = '\0';
        std::cout << "Clients input was: " << buffer << std::endl;
        if ((count = send(fd, buffer, sizeof(buffer), 0)) == -1) {
          std::cerr << "ERROR: send failed" << std::endl;
          close(fd);
          break;
        }
        std::memset(buffer, '\0', sizeof(buffer));
      }
      if (count == 0) {
        std::cout << "Client disconnected" << std::endl;
        close(fd);
      } else if (count == -1 && errno != EAGAIN) {
        std::cerr << "ERROR: recv failed" << std::endl;
        close(fd);
      }
    }
  }
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

  //
  //
  // PUT REQUEST HANDLE HERE AND THEN PASS IT TO THE HANDLE_EVENT
  // HANDLER WILL CREATE STORE AND 1 TIME AND THEN IN HANDLE METHOD PARSE IT AND COMMAND EXECUTE
  //
  //

  while (true) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, WAIT_TIMEOUT);
    if (nfds == 0) {
      std::cout << "Timeout 20 hit" << std::endl;
      break;
    }

    for (int i{}; i < nfds; ++i) {
      if (events[i].data.fd == server_fd) {
        while (true) {
          sockaddr_in client_info{};
          socklen_t client_info_len = sizeof(client_info);
          int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_info), &client_info_len);

          if (client_fd == -1) {
            if (errno == EAGAIN) {
              //std::cerr << "ERROR: accept client failed--> " << std::strerror(errno) << std::endl;
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
}
