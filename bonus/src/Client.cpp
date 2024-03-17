#include "Client.hpp"
#include "Message.hpp"
#include "utils/Logger.hpp"
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signals.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client() : _fd(-1) { Logger::info("Client initialized"); }

Client::~Client() {
  if (_fd > 0) {
    close(_fd);
  }

  freeaddrinfo(_res);

  Logger::info("Client shutdown");
}

ssize_t Client::read() {
  fcntl(_fd, F_SETFL, O_NONBLOCK);
  char buffer[BUFFER_SIZE] = {0};

  ssize_t bytes = recv(_fd, buffer, BUFFER_SIZE, 0);
  if (bytes > 0) {
    _message.append(buffer);
    Logger::in(buffer);
  }

  return bytes;
}

std::vector<MessageData> Client::getMessages() {
  std::vector<MessageData> messages;

  size_t pos;
  while ((pos = _message.find("\r\n")) != std::string::npos) {
    std::string message = _message.substr(0, pos);
    _message.erase(0, pos + 2);
    messages.push_back(Message::parse(message));
  }

  return messages;
}

void Client::send(const std::string &message) {
  ::send(_fd, message.c_str(), message.size(), 0);
  ::send(_fd, "\r\n", 2, 0);
  Logger::out(message);
}

void Client::connect(const char *hostname, const char *port) {
  struct addrinfo hints, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname, port, &hints, &_res) != 0) {
    throw SocketException("Could not get address info");
  }

  for (p = _res; p != NULL; p = p->ai_next) {
    _fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (_fd < 0) {
      continue;
    }

    if (::connect(_fd, p->ai_addr, p->ai_addrlen) < 0) {
      close(_fd);
      continue;
    }

    break;
  }

  if (p == NULL) {
    throw SocketException("Could not connect to server");
  }

  Logger::info("Connected to server");
}

bool Client::authenticate(const std::string &username,
                          const std::string &password) {
  if (!password.empty()) {
    send("PASS " + password);
  }
  send("NICK " + username);
  send("USER " + username + " 0 * :" + username);

  while (receivedSignal != SIGINT) {
    ssize_t bytes = read();
    if (bytes <= 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue;
      }
      break;
    }

    std::vector<MessageData> messages = getMessages();
    for (std::vector<MessageData>::iterator it = messages.begin();
         it != messages.end(); ++it) {
      if (it->command == "001") {
        Logger::info("Authenticated with server");
        return true;
      }
    }
  }

  return false;
}

void Client::handle() {
  while (receivedSignal != SIGINT) {
    ssize_t bytes = read();
    if (bytes <= 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        continue;
      }
      break;
    }
    std::vector<MessageData> messages = getMessages();
    for (std::vector<MessageData>::iterator it = messages.begin();
         it != messages.end(); ++it) {
      // ...
    }
  }
}
