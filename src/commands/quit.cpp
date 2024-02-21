#include "Client.hpp"
#include "Server.hpp"

void quitCommand(const std::vector<std::string> params, Client &sender) {
  (void)params;
  sender.quit();
}
