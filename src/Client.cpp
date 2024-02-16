#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <sys/socket.h>
#include <unistd.h>

Client::Client(Server &server, int fd)
    : _server(server), _fd(fd), _bufferPos(0) {}

Client::~Client() { close(_fd); }

void Client::send(const std::string &message) {
  ::send(_fd, message.c_str(), message.size(), 0);
  ::send(_fd, "\r\n", 2, 0);
}

ssize_t Client::read(MessageData &messageData) {
  size_t carriageReturnPos = std::string(_buffer).find("\r\n", _bufferPos);
  size_t bytesRead;

  if (carriageReturnPos == std::string::npos) {
    _bufferPos = 0;
    memset(_buffer, 0, BUFFER_SIZE); // NOTE: Optimizable
    bytesRead = recv(_fd, _buffer, BUFFER_SIZE, 0);
    if (bytesRead <= 0)
      return bytesRead;
  }

  carriageReturnPos = std::string(_buffer).find("\r\n", _bufferPos);
  if (carriageReturnPos == std::string::npos)
    throw IRCComplianceException("Missing carriage return");
  std::string message =
      std::string(_buffer).substr(_bufferPos, carriageReturnPos - _bufferPos);
  messageData = parseMessage(message);
  bytesRead = carriageReturnPos - _bufferPos;
  _bufferPos = carriageReturnPos + 2; // We add 2 to skip the carriage return

  return bytesRead;
}

void Client::disconnect() { _server.removeClient(_fd); }

void Client::kick(std::string reason) {
  send(reason);
  disconnect();
}
