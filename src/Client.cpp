#include "Client.hpp"
#include "CONFIG.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include "utils/Logger.hpp"
#include <exception>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(Server &server, int fd)
    : _server(server), _fd(fd), _bufferPos(0), _registered(false),
      negotiating(false), password(""), nickname(""), username(""), mode(-1),
      hostname(""), realname("") {
  memset(_buffer, 0, BUFFER_SIZE);
}

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

size_t Client::read(MessageData &messageData) {
  size_t carriageReturnPos = std::string(_buffer).find("\r\n", _bufferPos);
  size_t bytesRead = 0;

  if (carriageReturnPos == std::string::npos) {
    _bufferPos = 0;
    memset(_buffer, 0, BUFFER_SIZE);
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
  if (_channels.size() >= MAX_CHANNELS) {
    send(ERR_TOOMANYCHANNELS(nickname, name));
    return;
  }

  Channel *channel = _server.getChannel(name);

  if (channel) {
    if (channel->modes & LIMIT_MODE &&
        channel->members().size() >= channel->maxMembers) {
      send(ERR_CHANNELISFULL(nickname, name));
      throw std::exception();
    }

    if (channel->modes & INVITE_MODE) {
      send(ERR_INVITEONLYCHAN(nickname, name));
      throw std::exception();
    }

    if (channel->modes & KEY_MODE && channel->key != key) {
      send(ERR_BADCHANNELKEY(nickname, name));
      throw std::exception();
    }

    if (channel->addMember(*this, false)) {
      _channels[name] = channel;
    }
  } else {
    channel = _server.addChannel(name, key);
    channel->addMember(*this, true);
  }

  _channels[name] = channel;
  channel->send(":" + nickname + "!" + username + "@" + hostname + " JOIN " +
                name);
  if (channel->topic.empty()) {
    send(RPL_NOTOPIC(nickname, name));
  } else {
    send(RPL_TOPIC(nickname, name, channel->topic));
  }
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
                          " PART " + name);
  } else {
    _channels[name]->send(":" + nickname + "!" + username + "@" + hostname +
                          " PART " + name + " :" + reason);
  }
  _channels[name]->removeMember(*this);
  _channels.erase(name);
}

void Client::kickChannel(const std::string &name) {
  _channels[name]->removeMember(*this);
  _channels.erase(name);
}

void Client::quit() { _server.removeClient(_fd); }
