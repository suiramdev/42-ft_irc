#include "Client.hpp"
#include "MistralAI.hpp"
#include "utils/Logger.hpp"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <signals.hpp>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port> [password]" << std::endl;
    return EXIT_FAILURE;
  }

  signal(SIGINT, signalHandler);

  try {
    MistralAI::connect("localhost", "8080");
  } catch (std::exception &e) {
    Logger::error("MitralAI: " + std::string(e.what()));
    return EXIT_FAILURE;
  }

  try {
    Client client(argv[1], argv[2]);
    if (client.authenticate("bot", argc > 3 ? argv[3] : "")) {
      client.handle();
    }
  } catch (std::exception &e) {
    Logger::error("Client: " + std::string(e.what()));
    return EXIT_FAILURE;
  }

  MistralAI::disconnect();

  return EXIT_SUCCESS;
}
