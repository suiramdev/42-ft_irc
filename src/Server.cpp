#include "Server.hpp"
#include "Client.hpp"
#include "CommandHandler.hpp"
#include "Message.hpp"
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server() {
  commandHandler = new CommandHandler();
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    throw SocketException("init");
  }

  std::cout << "Socket created" << std::endl;
}

Server::~Server() {
  _clients.clear();
  if (_fd > 0) {
    close(_fd);
  }
  delete commandHandler;

  std::cout << "Server shutdown" << std::endl;
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

  std::cout << "Socket bound to port " << port << std::endl;
}

Client *Server::addClient(int fd) {
  struct pollfd pfd;
  pfd.fd = fd;
  pfd.events = POLL_IN;
  pfd.revents = 0;
  _pfds.push_back(pfd);

  Client *client = new Client(*this, fd);
  _clients.insert(std::pair<int, Client *>(fd, client));

  return client;
}

void Server::removeClient(int fd) {
  for (size_t i = 0; i < _pfds.size(); i++) {
    if (_pfds[i].fd == fd) {
      _pfds.erase(_pfds.begin() + i);
    }
  }
  _clients.erase(fd);
}

void Server::handle() {
  fcntl(_fd, F_SETFL, O_NONBLOCK);

  while (true) {
    // Accept any incoming connections
    int client_fd = accept(_fd, NULL, NULL);
    if (client_fd < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
      throw SocketException("accept");
    } else if (client_fd > 0) { // A client connected
      addClient(client_fd);
      std::cout << "Client connected" << std::endl;
    }

    int poll_count = poll(_pfds.data(), _pfds.size(), 100); // Wait for events
    if (poll_count > 0) { // Events must be handled
      for (size_t i = 0; i < _pfds.size(); i++) {
        if (_pfds[i].revents & POLL_IN) { // A client has sent a message
          try {
            MessageData messageData;

            ssize_t bytes_read = _clients[_pfds[i].fd]->read(messageData);
            std::cout << bytes_read << " bytes read" << std::endl;
            if (bytes_read > 0) {
              logMessage(messageData);
              commandHandler->handleCommand(messageData.command,
                                            messageData.params,
                                            *_clients[_pfds[i].fd]);
            } else if (bytes_read == 0) {
              removeClient(_pfds[i].fd);
              std::cout << "Client disconnected" << std::endl;
            }
          } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
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

  std::cout << "Listening on port " << port << std::endl;

  handle();
}
