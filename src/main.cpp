#include "CommandHandler.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include "signals.hpp"
#include "utils/Logger.hpp"
#include <csignal>
#include <cstdlib>
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

  signal(SIGINT, signalHandler);

  try {
    Server server(argv[2]);
    CommandHandler *commandHandler = server.commandHandler;
    commandHandler->registerCommand("CAP", capCommand, false);
    commandHandler->registerCommand("PASS", passCommand, false, 1);
    commandHandler->registerCommand("NICK", nickCommand, false);
    commandHandler->registerCommand("USER", userCommand, false, 4);
    commandHandler->registerCommand("PING", pingCommand, true, 1);
    commandHandler->registerCommand("JOIN", joinCommand, true, 1);
    commandHandler->registerCommand("PART", partCommand, true, 1);
    commandHandler->registerCommand("QUIT", quitCommand);
    commandHandler->registerCommand("PRIVMSG", privmsgCommand);
    commandHandler->registerCommand("KICK", kickCommand, false, 2);
    commandHandler->registerCommand("TOPIC", topicCommand, false, 1);
    commandHandler->registerCommand("MODE", modeCommand, false, 1);
    commandHandler->registerCommand("INVITE", inviteCommand, false, 2);
    server.listen(atoi(argv[1]));
  } catch (std::exception &e) {
    Logger::error(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
