#include "Message.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// TODO: Refactor this function
MessageData parseMessage(const std::string &rawMessage) {
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
      getline(stream, token, '\r'); // Assuming message ends with "\r\n",
      trailingParam += token;
      messageData.params.push_back(trailingParam);
      break;
    } else {
      messageData.params.push_back(token);
    }
  }

  return messageData;
}

void logMessage(MessageData messageData) {
  std::cout << "Prefix: " << messageData.prefix << std::endl;
  std::cout << "Command: " << messageData.command << std::endl;
  std::cout << "Params: " << std::endl;
  for (size_t i = 0; i < messageData.params.size(); i++) {
    std::cout << messageData.params[i] << std::endl;
  }
}
