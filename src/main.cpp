#include "CommandHandler.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return EXIT_FAILURE;
  }

  // Extract the port and password from the command line arguments
  const int PORT = atoi(argv[1]);
  const std::string PASSWORD = argv[2];

  Server *server = NULL;

  try {
    server = new Server();
    CommandHandler *hookHandler = server->commandHandler;
    hookHandler->registerCommand("CAP", capCommand);
    server->listen(PORT);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  delete server;

  return EXIT_SUCCESS;
}
