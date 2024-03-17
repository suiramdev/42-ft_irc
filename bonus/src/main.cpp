#include "Client.hpp"
#include "utils/Logger.hpp"
#include <cstdlib>
#include <exception>
#include <signals.hpp>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port> (password)" << std::endl;
    return EXIT_FAILURE;
  }

  signal(SIGINT, signalHandler);

  try {
    Client client;
    client.connect(argv[1], argv[2]);
    if (client.authenticate("bot", argc > 3 ? argv[3] : "")) {
      client.handle();
    }
  } catch (std::exception &e) {
    Logger::error(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
