#include "Client.hpp"
#include "Server.hpp"
#include <string>

void userCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 4) {
    sender.send("461 ERR_NEEDMOREPARAMS USER :Not enough parameters");
    return;
  }

  if (sender.isRegistered()) {
    sender.send("462 ERR_ALREADYREGISTRED :You may not reregister");
    return;
  }

  sender.username = params[0];
  sender.realname = params[3];

  sender.attemptRegister();

  return;
}
