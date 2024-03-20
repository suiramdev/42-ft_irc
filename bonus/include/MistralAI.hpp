#pragma once

#include "Socket.hpp"
#include <netdb.h>
#include <string>

class MistralAI {
private:
  static Socket *_socket;

public:
  /**
   * @brief Connect to a server
   *
   * @param hostname The hostname of the server
   * @param port The port to connect to
   */
  static void connect(const char *hostname, const char *port);

  /**
   * @brief Disconnect from the server
   */
  static void disconnect();

  /**
   * @brief Prompt the server with a message
   *
   * @param message The message to send
   * @return The response from the server
   */
  static std::string prompt(const std::string &message);
};
