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

  Server *server = NULL;

  try {
    server = new Server(argv[2]);
    CommandHandler *hookHandler = server->commandHandler;
    hookHandler->registerCommand("CAP", capCommand, false);
    hookHandler->registerCommand("PASS", passCommand, false);
    hookHandler->registerCommand("NICK", nickCommand, false);
    hookHandler->registerCommand("USER", userCommand, false);
    hookHandler->registerCommand("PING", pingCommand);
    hookHandler->registerCommand("JOIN", joinCommand);
    hookHandler->registerCommand("QUIT", quitCommand);
    hookHandler->registerCommand("PRIVMSG", privmsgCommand);
    server->listen(atoi(argv[1]));
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  delete server;

  return EXIT_SUCCESS;
}
