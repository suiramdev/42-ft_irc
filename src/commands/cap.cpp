#include "Client.hpp"
#include <string>

void capCommand(const std::vector<std::string> params, Client &sender) {
  if (params[0] == "LS") {
    sender.negotiating = true;
    sender.send("CAP * LS :");
  } else if (params[0] == "END") {
    if (sender.negotiating) {
      sender.attemptRegister();
      sender.negotiating = false;
    }
  }
}
