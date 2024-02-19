#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

Channel::Channel(Server &server, std::string name)
    : _server(server), _name(name) {}

Channel::~Channel() { _clients.clear(); }

ChannelClient Channel::addClient(Client &client) {
  if (_clients.find(client.fd()) != _clients.end()) {
    throw std::runtime_error("Client already exists");
  }

  ChannelClient channelClient = {.privileged = _clients.size() <= 0,
                                 .client = &client};

  _clients.insert(std::pair<int, ChannelClient>(client.fd(), channelClient));

  return channelClient;
}

bool Channel::hasClient(Client &client) {
  return _clients.find(client.fd()) != _clients.end();
}

void Channel::removeClient(Client &client) {
  _clients.erase(client.fd());

  if (_clients.size() <= 0) {
    _server.removeChannel(_name);
  }
}

void Channel::send(const std::string &message) {
  for (std::map<int, ChannelClient>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    it->second.client->send(message);
  }
}

void Channel::send(const std::string &message, Client &sender) {
  for (std::map<int, ChannelClient>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    if (it->second.client->fd() != sender.fd()) {
      it->second.client->send(message);
    }
  }
}
