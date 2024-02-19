#include "CommandHandler.hpp"
#include "Client.hpp"

CommandHandler::CommandHandler() {}

CommandHandler::~CommandHandler() {}

void CommandHandler::registerCommand(const std::string name,
                                     command_callback callback,
                                     bool authRequired) {
  _commands[name] = (Command){callback, authRequired};
}

void CommandHandler::handleCommand(const std::string name,
                                   const std::vector<std::string> params,
                                   Client &client) {
  if (_commands.find(name) == _commands.end()) {
    return;
  }

  if (_commands[name].authRequired && !client.isRegistered()) {
    client.send("451 ERR_NOTREGISTERED :You have not registered");
    return;
  }

  _commands[name].callback(params, client);
}
