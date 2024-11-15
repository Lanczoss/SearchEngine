#include "reactor/EchoServer.h"

int main(int argc, char **argv) {
  EchoServer echo(4, 10, argv[1], argv[2]);

  echo.start();
  echo.stop();

  return 0;
}
