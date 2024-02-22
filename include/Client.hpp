#pragma once

#define BUFFER_SIZE 512

#include "Message.hpp"
#include <map>
#include <string>

class Server;
class Channel;

class Client {
private:
  Server &_server;
  int _fd;
  char _buffer[BUFFER_SIZE];
  size_t _bufferPos;
  bool _registered;
  std::map<std::string, Channel *> _channels;

public:
  bool negotiating;
  std::string password;
  std::string nickname;
  std::string username;
  int mode;
  std::string hostname;
  std::string realname;

  Client(Server &server, int fd);
  ~Client();

  const int &fd() const { return _fd; }

  Server &server() const { return _server; }

  std::map<std::string, Channel *> &channels() { return _channels; }

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
  size_t read(MessageData &messageData);

  /**
   * @brief Join a channel
   *
   * @param channel The channel to join
   * @param key The key to use, if the channel is password protected
   */
  void joinChannel(const std::string &name, const std::string &key = "");

  /**
   * @brief Part a channel
   *
   * @param channel The channel to part
   */
  void partChannel(const std::string &name, const std::string &reason = "");

  /**
   * @brief Disconnect the client
   */
  void quit();
};
