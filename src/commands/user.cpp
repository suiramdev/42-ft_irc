#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void userCommand(const std::vector<std::string> params, Client &sender) {
  if (sender.isRegistered()) {
    sender.send(ERR_ALREADYREGISTRED(sender.nickname));
    return;
  }

  sender.username = params[0];
  sender.realname = params[3];

  sender.attemptRegister();
}
