#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void nickCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send(ERR_NONICKNAMEGIVEN(sender.nickname));
    return;
  }

  sender.nickname = params[0];
}
