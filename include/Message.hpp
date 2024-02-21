#pragma once

#include <string>

typedef struct {
  std::string prefix;
  std::string command;
  std::vector<std::string> params;
} MessageData;

class IRCComplianceException : public std::exception {
private:
  std::string _msg;

public:
  IRCComplianceException(std::string message) throw()
      : _msg("Error: IRC compliance: " + message) {}
  virtual ~IRCComplianceException() throw() {}
  virtual const char *what() const throw() { return _msg.c_str(); }
};

/**
 * @brief Parse an IRC message
 *
 * @param rawMessage The raw message
 * Expected format: [:<prefix>] <command> [param1] [param2] ... [:<trailing>]
 * \r\n
 * @return The parsed message data
 */
MessageData parseMessage(const std::string &rawMessage);
