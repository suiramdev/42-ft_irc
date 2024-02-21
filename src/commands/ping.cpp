#include "Client.hpp"
#include "Server.hpp"
#include <string>

void pingCommand(const std::vector<std::string> params, Client &sender) {
  sender.send("PONG " + params[0]);
}
