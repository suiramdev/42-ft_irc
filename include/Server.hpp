#pragma once
#include "CommandHandler.hpp"
#include <exception>
#include <map>
#include <string>
#include <sys/poll.h>
#include <vector>

#define BACKLOG 10

class Client;
class Channel;
class HookHandler;

class Server {
private:
  int _fd;
  std::map<int, Client *> _clients;
  std::vector<struct pollfd> _pfds;
  std::map<std::string, Channel *> _channels;

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
  /* std::string hostname; */
  std::string password;
  CommandHandler *commandHandler;

  Server(std::string password);
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
   * @brief Get a client by file descriptor
   *
   * @param nickname The client's nickname
   * @return The client
   */
  Client *getClient(std::string nickname);

  /**
   * @brief Remove a client from the server
   * @param fd The client's file descriptor
   */
  void removeClient(int fd);

  /**
   * @brief Get a channel by name
   *
   * @param name The name of the channel
   * @param key The key to the channel (optional)
   * @return The channel
   */
  Channel *addChannel(std::string name);

  /**
   * @brief Get a channel by name, with a key
   *
   * @param name The name of the channel
   * #param key The key to the channel
   * @return The channel
   */
  Channel *addChannel(std::string name, std::string key);

  /**
   * @brief Get a channel by name
   *
   * @param name The name of the channel
   * @return The channel
   */
  Channel *getChannel(std::string name);

  /**
   * @brief Remove a channel by name
   *
   * @param name The name of the channel
   */
  void removeChannel(std::string name);

  /**
   * @brief Broadcast a message to all clients
   *
   * @param message The message to broadcast
   */
  void broadcast(const std::string &message);

  class SocketException : public std::exception {
  private:
    std::string _msg;

  public:
    SocketException(std::string message) throw() : _msg("socket: " + message) {}
    virtual ~SocketException() throw() {}
    virtual const char *what() const throw() { return _msg.c_str(); }
  };
};
