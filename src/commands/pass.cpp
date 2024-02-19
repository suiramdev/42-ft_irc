#include "Client.hpp"
#include "Server.hpp"
#include <string>

void passCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("461 ERR_NEEDMOREPARAMS PASS :Not enough parameters");
    return;
  }

  if (sender.isRegistered()) {
    sender.send("462 ERR_ALREADYREGISTRED :You may not reregister");
    return;
  }

  sender.password = params[0];

  sender.attemptRegister();

  return;
}
