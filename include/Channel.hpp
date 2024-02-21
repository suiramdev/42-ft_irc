#pragma once

#include <map>
#include <string>

typedef enum {
  BAN_CHANNEL = 0x1,
  LIMIT_CHANNEL = 0x2,
  INVITE_ONLY = 0x3,
  KEY_CHANNEL = 0x4,
} ChannelMode;

class Server;
class Client;

class Channel {
private:
  Server &_server;
  const std::string _name;
  std::map<int, Client *> _members;
  std::map<int, Client *> _operators;

public:
  ChannelMode mode;
  std::string key;
  std::string topic;
  unsigned long maxMembers;

  Channel(Server &server, std::string name);
  ~Channel();

  Server &server() const { return _server; }

  const std::string &name() const { return _name; }

  const std::map<int, Client *> &members() const { return _members; }

  const std::map<int, Client *> &operators() const { return _operators; }

  /**
   * @brief Add a member to the channel
   *
   * @param client The client to add
   * @param privileged Whether the client should be privileged
   * @return true if the client was added, false otherwise
   */
  bool addMember(Client &client, bool privileged = false);

  /**
   * @brief Check if a client is a member of the channel
   *
   * @param client The client to check
   * @return true if the client is a member, false otherwise
   */
  bool hasMember(Client &client);

  /**
   * @brief Remove a member from the channel
   *
   * @param client The client to remove
   */
  void removeMember(Client &client);

  /**
   * @brief Check if a client is an operator of the channel
   *
   * @param client The client to check
   * @return true if the client is an operator, false otherwise
   */
  bool isOperator(Client &client);

  /**
   * @brief Set a client as an operator of the channel
   *
   * @param client The client to set
   * @param privileged Whether the client should be privileged
   */
  void setOperator(Client &client, bool privileged = true);

  /**
   * @brief Send a message to the channel
   *
   * @param message The message to send
   */
  void send(const std::string &message);

  /**
   * @brief Send a message to the channel, excluding the sender
   *
   * @param message The message to send
   * @param sender The client that sent the message
   */
  void send(const std::string &message, Client &sender);
};
