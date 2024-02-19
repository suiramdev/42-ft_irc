#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

void quitCommand(const std::vector<std::string> params, Client &sender) {
  (void)params;
  sender.disconnect();

  std::cout << "Client disconnected" << std::endl;

  return;
}
