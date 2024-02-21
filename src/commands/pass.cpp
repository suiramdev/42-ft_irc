#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void passCommand(const std::vector<std::string> params, Client &sender) {
  if (sender.isRegistered()) {
    sender.send(ERR_ALREADYREGISTRED(sender.nickname));
    return;
  }

  sender.password = params[0];

  sender.attemptRegister();
}
