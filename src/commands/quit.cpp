#include "Client.hpp"
#include "Server.hpp"
#include "utils/Logger.hpp"

void quitCommand(const std::vector<std::string> params, Client &sender) {
  (void)params;
  sender.quit();
}
