#pragma once

#include <map>
#include <string>

class Server;
class Client;

typedef struct {
  bool privileged;
  Client *client;
} ChannelClient;

class Channel {
private:
  Server &_server;
  const std::string _name;
  std::map<int, ChannelClient> _clients;

public:
  std::string key;
  std::string topic;

  Channel(Server &server, std::string name);
  ~Channel();

  Server &server() const { return _server; }

  const std::string &name() const { return _name; }

  const std::map<int, ChannelClient> &clients() const { return _clients; }

  /**
   * @brief Add a client to the channel
   *
   * @param client The client to add
   * @return The channel client
   */
  ChannelClient addClient(Client &client);

  /**
   * @brief Check if a client is in the channel
   *
   * @param client The client to check
   * @return true if the client is in the channel, false otherwise
   */
  bool hasClient(Client &client);

  /**
   * @brief Remove a client from the channel
   *
   * @param client The client to remove
   */
  void removeClient(Client &client);

  /**
   * @brief Send a message to the channel
   *
   * @param message The message to send
   */
  void send(const std::string &message);

  /**
   * @brief Send a message to the channel
   *
   * @param message The message to send
   * @param sender The client that sent the message
   */
  void send(const std::string &message, Client &sender);
};
