#include "Client.hpp"
#include "CONFIG.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include "utils/Logger.hpp"
#include <cstring>
#include <exception>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client(Server &server, int fd)
    : _server(server), _fd(fd), _message(""), _registered(false),
      negotiating(false), password(""), nickname(""), username(""), mode(-1),
      hostname(""), realname("") {}

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
  size_t carriageReturn = _message.find("\r\n");
  if (carriageReturn == std::string::npos) {
    char buffer[BUFFER_SIZE] = {0};

    ssize_t bytesRead = recv(_fd, buffer, BUFFER_SIZE, 0);
    if (bytesRead <= 0) {
      return bytesRead;
    }

    _message.append(buffer);

    carriageReturn = _message.find("\r\n");
    if (carriageReturn == std::string::npos) {
      return -1;
    }
  }

  Logger::in(_message.substr(0, carriageReturn));
  messageData = Message::parse(_message.substr(0, carriageReturn));
  _message = _message.substr(carriageReturn + 2);

  return carriageReturn;
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

    if (channel->modes & INVITE_MODE && !channel->isInvited(*this)) {
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
