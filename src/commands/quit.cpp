#include "Client.hpp"
#include "Server.hpp"
#include <iostream>

void quitCommand(const std::vector<std::string> params, Client &sender) {
  (void)params;
  sender.quit();

  std::cout << "Client disconnected" << std::endl;

  return;
}
