#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "CommandHandler.hpp"
#include "Message.hpp"
#include "signals.hpp"
#include "utils/Logger.hpp"
#include <csignal>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(std::string password) : password(password) {
  commandHandler = new CommandHandler();
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    throw SocketException("init");
  }

  Logger::info("Server initialized");
}

Server::~Server() {
  // Delete all clients
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    delete it->second;
  }
  // Delete all channels
  for (std::map<std::string, Channel *>::iterator it = _channels.begin();
       it != _channels.end(); ++it) {
    delete it->second;
  }

  if (_fd > 0) {
    // Close the server's socket
    close(_fd);
  }
  delete commandHandler;

  Logger::info("Server shutdown");
}

void Server::setReuseAddr() {
  int optval = 1;

  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    throw SocketException("setsockopt");
  }
}

void Server::bind(int port) {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (::bind(_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    throw SocketException("bind");
  }

  std::stringstream ss;
  ss << "Server bound to port " << port;

  Logger::info(ss.str());
}

Client *Server::addClient(int fd) {
  if (_clients.find(fd) != _clients.end()) {
    throw std::runtime_error("Client already exists");
  }

  struct pollfd pfd;
  pfd.fd = fd;
  pfd.events = POLLIN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  Client *client = new Client(*this, fd);
  _clients.insert(std::pair<int, Client *>(fd, client));

  return client;
}

Client *Server::getClient(std::string nickname) {
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    if (it->second->nickname == nickname) {
      return it->second;
    }
  }

  return NULL;
}

void Server::removeClient(int fd) {
  for (size_t i = 0; i < _pfds.size(); i++) {
    if (_pfds[i].fd == fd) {
      _pfds.erase(_pfds.begin() + i);
    }
  }
  Client *client = _clients[fd];
  if (client && client->isRegistered()) {
    Logger::info(client->nickname + " has quit");
  } else {
    Logger::info("A client has quit");
  }

  delete _clients[fd];
  _clients.erase(fd);
}

void Server::handle() {
  fcntl(_fd, F_SETFL, O_NONBLOCK);

  while (receivedSignal != SIGINT) {
    // Accept any incoming connections
    int client_fd = accept(_fd, NULL, NULL);
    if (client_fd < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
      throw SocketException("accept");
    } else if (client_fd > 0) { // A client connected
      addClient(client_fd);
      Logger::info("Client connected");
    }

    int poll_count = poll(_pfds.data(), _pfds.size(), 100); // Wait for events
    if (poll_count > 0) { // Events must be handled
      for (size_t i = 0; i < _pfds.size(); i++) {
        if (_pfds[i].revents & POLLIN) { // A client has sent a message

          ssize_t bytes_read = _clients[_pfds[i].fd]->read();
          if (bytes_read <= 0) {
            removeClient(_pfds[i].fd);
            continue;
          }

          std::vector<MessageData> messages =
              _clients[_pfds[i].fd]->getMessages();
          for (std::vector<MessageData>::iterator it = messages.begin();
               it != messages.end(); ++it) {
            commandHandler->handleCommand(it->command, it->params,
                                          *_clients[_pfds[i].fd]);
          }
        }
      }
    }
  }
}

void Server::listen(int port) {
  setReuseAddr();
  bind(port);

  if (::listen(_fd, BACKLOG) < 0) {
    throw SocketException("listen");
  }

  std::stringstream ss;
  ss << "Server listening on port " << port;

  Logger::info(ss.str());

  handle();
}

Channel *Server::addChannel(std::string name) {
  if (_channels.find(name) != _channels.end()) {
    throw std::runtime_error("Channel already exists");
  }

  Channel *channel = new Channel(*this, name);
  _channels.insert(std::pair<std::string, Channel *>(name, channel));

  return channel;
}

Channel *Server::addChannel(std::string name, std::string key) {
  Channel *channel = addChannel(name);
  channel->key = key;

  return channel;
}

Channel *Server::getChannel(std::string name) {
  if (_channels.find(name) == _channels.end()) {
    return NULL;
  }

  return _channels[name];
}

void Server::removeChannel(std::string name) {
  if (_channels.find(name) == _channels.end()) {
    throw std::runtime_error("Channel does not exist");
  }

  delete _channels[name];
  _channels.erase(name);
}

void Server::broadcast(const std::string &message) {
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    it->second->send(message);
  }
}
