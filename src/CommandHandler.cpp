#include "CommandHandler.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"

CommandHandler::CommandHandler() {}

CommandHandler::~CommandHandler() {}

void CommandHandler::registerCommand(const std::string name,
                                     command_callback callback,
                                     bool authRequired,
                                     unsigned long minParams) {
  _commands[name] = (Command){callback, authRequired, minParams};
}

void CommandHandler::handleCommand(const std::string name,
                                   const std::vector<std::string> params,
                                   Client &sender) {
  if (_commands.find(name) == _commands.end()) {
    sender.send(ERR_UNKNOWNCOMMAND(sender.nickname, name));
    return;
  }

  if (_commands[name].authRequired && !sender.isRegistered()) {
    sender.send(ERR_NOTREGISTERED(sender.nickname));
    return;
  }

  if (params.size() < _commands[name].minParams) {
    sender.send(ERR_NEEDMOREPARAMS(sender.nickname, name));
    return;
  }

  _commands[name].callback(params, sender);
}
