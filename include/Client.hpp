#pragma once

#define BUFFER_SIZE 512

#include "Message.hpp"
#include <string>

class Server;

class Client {
private:
  Server &_server;
  int _fd;
  char _buffer[BUFFER_SIZE];
  size_t _bufferPos;
  bool _registered;

public:
  std::string password;
  std::string nickname;
  std::string username;
  int mode;
  std::string realname;

  Client(Server &server, int fd);
  ~Client();

  const int &fd() const { return _fd; }

  const Server &server() const { return _server; }

  /**
   * @brief Send a message to the client
   *
   * @param message The message to send
   */
  void send(const std::string &message);

  /**
   * @brief Read a message from the client
   *
   * @param messageData The struct to store the message data in
   * @return The number of bytes read, 0 if the client has disconnected, or -1
   * if an error occurred
   * @throw IRCComplianceException If the message is not IRC compliant
   */
  ssize_t read(MessageData &messageData);

  /**
   * @brief Disconnect the client
   */
  void disconnect();

  /**
   * @brief Kick the client
   *
   * @param reason The reason for the kick
   */
  void kick(std::string reason);

  /**
   * @brief Attempt to register the client
   */
  void attemptRegister();

  /**
   * @brief Check if the client is registered
   *
   * @return true if the client is registered, false otherwise
   */
  bool isRegistered() const { return _registered; }
};
