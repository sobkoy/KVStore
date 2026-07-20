#include "server.h"

int main(int argc, char* argv[]) {
  Server server(8080);
  server.Run();


  return 0;
}
