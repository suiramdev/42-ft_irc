#include "Client.hpp"
#include <string>

void joinCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("461 ERR_NEEDMOREPARAMS JOIN :Not enough parameters");
    return;
  }

  return;
}
