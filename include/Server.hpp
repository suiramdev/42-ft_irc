#pragma once
#include "CommandHandler.hpp"
#include <exception>
#include <map>
#include <string>
#include <sys/poll.h>
#include <vector>

#define BACKLOG 10

class Client;
class HookHandler;

class Server {
private:
  int _fd;
  std::map<int, Client *> _clients;
  std::vector<struct pollfd> _pfds;

  /**
   * @brief Set the socket to reuse addresses
   */
  void setReuseAddr();
  /**
   * @brief Bind the server to a port
   *
   * @param port The port to bind to
   */
  void bind(int port);
  /**
   * @brief Handle incoming connections, and client messages
   */
  void handle();

public:
  CommandHandler *commandHandler;

  Server();
  ~Server();

  /**
   * @brief Get the server's file descriptor
   *
   * @return The server's file descriptor
   */
  const int &fd() const { return _fd; }
  /**
   * @brief Listen on a port
   *
   * @param port The port to listen on
   */
  void listen(int port);
  /**
   * @brief Add a client to the server
   *
   * @param fd The client's file descriptor
   * @return The client
   */
  Client *addClient(int fd);
  /**
   * @brief Get a client from the server
   *
   * @return The client
   */
  Client *getClient(int fd);
  /**
   * @brief Remove a client from the server
   * @param fd The client's file descriptor
   */
  void removeClient(int fd);

  class SocketException : public std::exception {
  private:
    std::string _msg;

  public:
    SocketException(std::string message) throw()
        : _msg("Error: socket: " + message) {}
    virtual ~SocketException() throw() {}
    virtual const char *what() const throw() { return _msg.c_str(); }
  };
};
