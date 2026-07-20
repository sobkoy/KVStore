#include "client.h"

int main(int argc, char* argv[]) {
  Client client("127.0.0.1", 8080);
  client.Connect();

  return 0;
}
