#include "Channel.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"

Channel::Channel(Server &server, std::string name)
    : _server(server), _name(name), modes(NONE_MODE), key(""), topic(""),
      topicSetter(NULL), topicSetTime(0), maxMembers(10) {}

Channel::~Channel() {}

bool Channel::addMember(Client &client, bool privileged) {
  if (modes == LIMIT_MODE && _members.size() >= maxMembers) {
    client.send(ERR_CHANNELISFULL(client.nickname, _name));
    return false;
  }

  _members[client.fd()] = &client;
  if (privileged) {
    _operators[client.fd()] = &client;
  }
  return true;
}

Client *Channel::getMember(const std::string &name) {
  for (std::map<int, Client *>::iterator it = _members.begin();
       it != _members.end(); ++it) {
    if (it->second->nickname == name) {
      return it->second;
    }
  }
  return NULL;
}

bool Channel::hasMember(Client &client) {
  return _members.find(client.fd()) != _members.end();
}

void Channel::removeMember(Client &client) {
  _members.erase(client.fd());
  _operators.erase(client.fd());

  if (_members.empty()) { // If the channel is empty, remove it from the server
    _server.removeChannel(_name);
  }
}

void Channel::inviteMember(Client &client) { _invited[client.fd()] = &client; }

void Channel::removeInvitation(Client &client) { _invited.erase(client.fd()); }

bool Channel::isInvited(Client &client) {
  return _invited.find(client.fd()) != _invited.end();
}

void Channel::setOperator(Client &client, bool privileged) {
  if (!hasMember(client)) {
    return;
  }

  if (privileged) {
    _operators[client.fd()] = &client;
  } else {
    _operators.erase(client.fd());
  }
}

bool Channel::isOperator(Client &client) {
  return _operators.find(client.fd()) != _operators.end();
}

void Channel::send(const std::string &message) {
  for (std::map<int, Client *>::iterator it = _members.begin();
       it != _members.end(); ++it) {
    it->second->send(message);
  }
}

void Channel::send(const std::string &message, Client &sender) {
  for (std::map<int, Client *>::iterator it = _members.begin();
       it != _members.end(); ++it) {
    if (it->second->fd() != sender.fd()) {
      it->second->send(message);
    }
  }
}
