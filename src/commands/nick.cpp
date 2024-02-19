#include "Client.hpp"
#include "Server.hpp"
#include <string>

void nickCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("461 ERR_NEEDMOREPARAMS NICK :Not enough parameters");
    return;
  }

  if (sender.isRegistered()) {
    sender.send("462 ERR_ALREADYREGISTRED :You may not reregister");
    return;
  }

  sender.nickname = params[0];

  sender.attemptRegister();

  return;
}
