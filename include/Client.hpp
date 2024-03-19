#pragma once

#define BUFFER_SIZE 512

#include "Message.hpp"
#include <map>
#include <string>
#include <sys/types.h>

class Server;
class Channel;

class Client {
private:
  Server &_server;
  int _fd;
  std::string _message;
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
   * @return The number of bytes read
   */
  ssize_t read();

  /**
   * @brief Get the message from the client buffer
   *
   * @return The messages
   */
  std::vector<Message> getMessages();

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
   * @param name The channel name to part
   * @param reason The reason for parting
   */
  void partChannel(const std::string &name, const std::string &reason = "");

  /**
   * @brief Kick the client from a channel
   *
   * @param name The channel name to kick from
   */
  void kickChannel(const std::string &name);

  /**
   * @brief Disconnect the client
   */
  void quit();
};
