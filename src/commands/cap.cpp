#include "Client.hpp"
#include <string>

void capCommand(const std::vector<std::string> params, Client &sender) {
  if (params[0] == "LS") {
    sender.send("CAP * LS :");
  } else if (params[0] == "END") {
    sender.send("CAP * ACK :");
  }
}
