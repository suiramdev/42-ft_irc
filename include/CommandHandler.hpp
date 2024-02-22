#pragma once

#include <map>
#include <string>
#include <vector>

class Client;

typedef void (*command_callback)(const std::vector<std::string>, Client &);

typedef struct {
  command_callback callback;
  bool authRequired;
  unsigned long minParams;
} Command;

class CommandHandler {
private:
  std::map<std::string, Command> _commands;

public:
  CommandHandler();
  ~CommandHandler();

  /**
   * @brief Register a command
   *
   * @param name Name of the command
   * @param callback Callback to be called when the command is fired
   */
  void registerCommand(const std::string name, command_callback callback,
                       bool authRequired = true, unsigned long minParams = 0);

  /**
   * @brief Handle a command
   *
   * @param name The command name to handle
   * @param messageData The message data
   * @param sender The client that sent the command
   */
  void handleCommand(const std::string name,
                     const std::vector<std::string> params, Client &sender);
};
