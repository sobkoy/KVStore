#pragma once

class Server {
public:
  explicit Server(const unsigned short port) : port_{port} {}

  void Run() const;

private:

  unsigned short port_;
};
