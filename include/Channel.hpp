#pragma once

#include <ctime>
#include <map>
#include <string>

typedef enum {
  NONE_MODE = 0,
  LIMIT_MODE = 1,
  INVITE_MODE = 2,
  KEY_MODE = 3,
  TOPIC_MODE = 4,
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
  int modes;
  std::string key;
  std::string topic;
  Client *topicSetter;
  std::time_t topicSetTime;
  unsigned long maxMembers;

  Channel(Server &server, std::string name);
  ~Channel();

  Server &server() const { return _server; }

  const std::string &name() const { return _name; }

  std::map<int, Client *> &members() { return _members; }

  std::map<int, Client *> &operators() { return _operators; }

  /**
   * @brief Add a member to the channel
   *
   * @param client The client to add
   * @param privileged Whether the client should be privileged
   * @return true if the client was added, false otherwise
   */
  bool addMember(Client &client, bool privileged = false);

  /**
   * @brief Get a member of the channel
   *
   * @param name The name of the client
   * @return The client
   */
  Client *getMember(const std::string &name);

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
