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
    CommandHandler *commandHandler = server->commandHandler;
    commandHandler->registerCommand("CAP", capCommand, false);
    commandHandler->registerCommand("PASS", passCommand, false, 1);
    commandHandler->registerCommand("NICK", nickCommand, false);
    commandHandler->registerCommand("USER", userCommand, false, 4);
    commandHandler->registerCommand("PING", pingCommand, true, 1);
    commandHandler->registerCommand("JOIN", joinCommand, true, 1);
    commandHandler->registerCommand("QUIT", quitCommand);
    commandHandler->registerCommand("PRIVMSG", privmsgCommand);
    server->listen(atoi(argv[1]));
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  delete server;

  return EXIT_SUCCESS;
}
