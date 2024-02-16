#pragma once

#include <map>
#include <string>

class Client;

typedef void (*command_callback)(const std::vector<std::string>, Client &);

class CommandHandler {
private:
  std::map<std::string, command_callback> _commands;

public:
  CommandHandler();
  ~CommandHandler();

  /**
   * @brief Register a command
   *
   * @param name Name of the command
   * @param callback Callback to be called when the command is fired
   */
  void registerCommand(const std::string name, command_callback callback);
  /**
   * @brief Handle a command
   *
   * @param command The command to handle
   * @param messageData The message data
   * @param sender The client that sent the command
   */
  void handleCommand(const std::string command,
                     const std::vector<std::string> params, Client &sender);
};
