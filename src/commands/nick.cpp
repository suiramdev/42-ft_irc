#include "Client.hpp"
#include "Server.hpp"
#include <string>

void nickCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("431 ERR_NONICKNAMEGIVEN :No nickname given");
    return;
  }

  sender.nickname = params[0];

  return;
}
