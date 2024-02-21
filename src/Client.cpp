#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include "utils/Logger.hpp"
#include <exception>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(Server &server, int fd)
    : _server(server), _fd(fd), _bufferPos(0), mode(-1) {}

Client::~Client() {
  if (_fd > 0) {
    close(_fd);
  }
}

void Client::attemptRegister() {
  if (password.empty() || nickname.empty() || username.empty() ||
      realname.empty()) {
    return;
  }

  if (password != _server.password) {
    send(ERR_PASSWDMISMATCH(nickname));
    return;
  }

  _registered = true;

  send(RPL_WELCOME(nickname));
}

void Client::send(const std::string &message) {
  ::send(_fd, message.c_str(), message.size(), 0);
  ::send(_fd, "\r\n", 2, 0);

  Logger::out(message);
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
  Logger::in(message);
  bytesRead = carriageReturnPos - _bufferPos;
  _bufferPos = carriageReturnPos + 2; // We add 2 to skip the carriage return

  return bytesRead;
}

void Client::joinChannel(const std::string &name, const std::string &key) {
  Channel *channel = _server.getChannel(name);

  if (channel) {
    // TODO: Reject if the client is already is not invited
    if (channel->mode == KEY_CHANNEL && key != key) {
      throw std::exception();
      send(ERR_BADCHANNELKEY(nickname, name));
      return;
    }

    channel->addMember(*this, false);
    _channels[name] = channel;
  } else {
    channel = _server.addChannel(name, key);
    channel->addMember(*this, true);
  }

  _channels[name] = channel;
  // TODO: Send JOIN message to channel, and topic, ...
}

void Client::partChannel(const std::string &name, const std::string &reason) {
  if (!_server.getChannel(name)) {
    send(ERR_NOSUCHCHANNEL(nickname, name));
    return;
  }

  if (_channels.find(name) == _channels.end()) {
    send(ERR_NOTONCHANNEL(nickname, name));
    return;
  }

  if (reason.empty()) {
    _channels[name]->send(":" + nickname + "!" + username + "@" + hostname +
                              " PART #" + name,
                          *this);
  } else {
    _channels[name]->send(":" + nickname + "!" + username + "@" + hostname +
                              " PART #" + name + " :" + reason,
                          *this);
  }
  _channels[name]->removeMember(*this);
  _channels.erase(name);
}

void Client::quit() { _server.removeClient(_fd); }
