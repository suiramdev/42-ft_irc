#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {}

CommandHandler::~CommandHandler() {}

void CommandHandler::registerCommand(const std::string name,
                                     command_callback callback) {
  _commands[name] = callback;
}

void CommandHandler::handleCommand(const std::string command,
                                   const std::vector<std::string> params,
                                   Client &client) {
  if (_commands.find(command) == _commands.end()) {
    return;
  }

  _commands[command](params, client);
}
