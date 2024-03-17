#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
  std::string prefix;
  std::string command;
  std::vector<std::string> params;
} MessageData;

class Message {
public:
  /**
   * @brief Parse an IRC message
   *
   * @param rawMessage The raw message
   * Expected format: [:<prefix>] <command> [param1] [param2] ... [:<trailing>]
   * \r\n
   * @return The parsed message data
   */
  static MessageData parse(const std::string &rawMessage) {
    MessageData messageData;
    std::istringstream stream(rawMessage);
    std::string token;
    size_t pos = 0;

    // Check for prefix
    if (rawMessage[pos] == ':') {
      pos = rawMessage.find(' ', pos);
      if (pos == std::string::npos)
        throw IRCComplianceException("Missing command");
      messageData.prefix = rawMessage.substr(1, pos - 1);
      stream.seekg(pos + 1);
    }

    // Extract command
    stream >> messageData.command;
    if (messageData.command.empty())
      throw IRCComplianceException("Missing command");

    // Extract parameters
    while (stream >> token) {
      if (token[0] == ':') {
        // The rest of the line is a single trailing parameter
        std::string trailingParam = token.substr(1);
        if (getline(stream, token, '\r')) {
          trailingParam += token; // Append the rest of the line
        }
        messageData.params.push_back(trailingParam);
        break;
      } else {
        messageData.params.push_back(token);
      }
    }

    return messageData;
  }

  class IRCComplianceException : public std::exception {
  private:
    std::string _msg;

  public:
    IRCComplianceException(std::string message) throw()
        : _msg("IRC compliance: " + message) {}
    virtual ~IRCComplianceException() throw() {}
    virtual const char *what() const throw() { return _msg.c_str(); }
  };
};
