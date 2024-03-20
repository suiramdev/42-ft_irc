#include "Client.hpp"
#include "Message.hpp"
#include "MistralAI.hpp"
#include "utils/Logger.hpp"
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signals.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client() : _socket(new Socket()) { Logger::info("Client initialized"); }

Client::Client(const char *hostname, const char *port) : _socket(new Socket()) {
  connect(hostname, port);
}

Client::~Client() {
  delete _socket;

  Logger::info("Client shutdown");
}

void Client::connect(const char *hostname, const char *port) {
  _socket->connect(hostname, port);
  Logger::info("Connected to server");
}

ssize_t Client::read() {
  fcntl(_socket->getFd(), F_SETFL, O_NONBLOCK);
  char buffer[BUFFER_SIZE] = {0};

  ssize_t bytes = recv(_socket->getFd(), buffer, BUFFER_SIZE, 0);
  if (bytes > 0) {
    _message.append(buffer);
    Logger::in(buffer);
  }

  return bytes;
}

std::vector<Message> Client::getMessages() {
  std::vector<Message> messages;

  size_t pos;
  while ((pos = _message.find("\r\n")) != std::string::npos) {
    std::string message = _message.substr(0, pos);
    _message.erase(0, pos + 2);
    messages.push_back(Message(message));
  }

  return messages;
}

void Client::send(const std::string &message) {
  ::send(_socket->getFd(), message.c_str(), message.size(), 0);
  ::send(_socket->getFd(), "\r\n", 2, 0);
  Logger::out(message);
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

    std::vector<Message> messages = getMessages();
    for (std::vector<Message>::iterator it = messages.begin();
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
    std::vector<Message> messages = getMessages();
    for (std::vector<Message>::iterator it = messages.begin();
         it != messages.end(); ++it) {
      if (it->command == "PRIVMSG") {
        send("PRIVMSG " + it->nickname + " :" +
             MistralAI::prompt(it->params[1]));
      }
    }
  }
}
