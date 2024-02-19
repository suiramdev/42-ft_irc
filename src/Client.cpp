#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(Server &server, int fd)
    : _server(server), _fd(fd), _bufferPos(0), mode(-1) {}

Client::~Client() { close(_fd); }

void Client::send(const std::string &message) {
  ::send(_fd, message.c_str(), message.size(), 0);
  ::send(_fd, "\r\n", 2, 0);

  std::cout << "Sent message: " << message << std::endl;
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
  std::cout << "Received message: " << message << std::endl;
  bytesRead = carriageReturnPos - _bufferPos;
  _bufferPos = carriageReturnPos + 2; // We add 2 to skip the carriage return

  return bytesRead;
}

void Client::attemptRegister() {
  if (password.empty() || nickname.empty() || username.empty() || mode > 0 ||
      hostname.empty() || realname.empty()) {
    return;
  }

  if (password != _server.password) {
    send("464 ERR_PASSWDMISMATCH :Password incorrect");
    return;
  }

  _registered = true;

  send("001 " + nickname + " :Welcome to the Internet Relay Network " +
       nickname + "!" + username + "@" + hostname);
}

void Client::disconnect() { _server.removeClient(_fd); }
