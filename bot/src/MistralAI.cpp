#include "MistralAI.hpp"
#include "Socket.hpp"
#include "utils/Logger.hpp"
#include <string>
#include <unistd.h>

Socket *MistralAI::_socket = NULL;

void MistralAI::connect(const char *hostname, const char *port) {
  if (_socket == NULL) {
    _socket = new Socket();
  }

  _socket->connect(hostname, port);
  Logger::info("MistralAI connected to server");
}

void MistralAI::disconnect() {
  delete _socket;
  _socket = NULL;
}

std::string MistralAI::prompt(const std::string &message) {
  if (_socket == NULL || _socket->getFd() == -1) {
    return "Internal error: not connected to server";
  }

  send(_socket->getFd(), message.c_str(), message.size(), 0);
  char buffer[1024] = {0};

  ssize_t bytes = recv(_socket->getFd(), buffer, 1024, 0);
  if (bytes > 0) {
    return std::string(buffer);
  }

  return "Internal error: could not receive response";
}
