#pragma once

#include "Message.hpp"
#include "Socket.hpp"
#include <exception>
#include <netdb.h>
#include <string>

#define BUFFER_SIZE 512

class Client {
private:
  Socket *_socket;
  std::string _message;

public:
  Client();
  Client(const char *hostname, const char *port);
  ~Client();

  /**
   * @brief Connect to a server
   *
   * @param hostname The hostname of the server
   * @param port The port to connect to
   */
  void connect(const char *hostname, const char *port);

  /**
   * @brief Read incoming messages from the server
   *
   * @return The number of bytes read
   */
  ssize_t read();

  /**
   * @brief Get the messages that have been read
   *
   * @return The messages
   */
  std::vector<Message> getMessages();

  /**
   * @brief Send a message to the server
   *
   * @param message The message to send
   */
  void send(const std::string &message);

  /**
   * @brief Authenticate with the server
   *
   * @param username The username to authenticate with
   * @param password The password to authenticate with (optional)
   * @return true if the authentication was successful, false otherwise
   */
  bool authenticate(const std::string &username,
                    const std::string &password = "");

  /**
   * @brief Handle incoming messages from the server
   */
  void handle();

  class SocketException : public std::exception {
  private:
    std::string _msg;

  public:
    SocketException(std::string message) throw() : _msg("socket: " + message) {}
    virtual ~SocketException() throw() {}
    virtual const char *what() const throw() { return _msg.c_str(); }
  };
};
