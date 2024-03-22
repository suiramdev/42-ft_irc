#include "Message.hpp"

#include <sstream>
#include <string>

Message::Message(const std::string &command,
                 const std::vector<std::string> &params,
                 const std::string &nickname, const std::string &username,
                 const std::string &hostname)
    : nickname(nickname), username(username), hostname(hostname),
      command(command), params(params) {}

Message::Message(const std::string &rawMessage)
    : nickname(""), username(""), hostname(""), command("") {
  std::istringstream iss(rawMessage);
  std::string token;

  iss >> token;
  if (token[0] == ':') {
    size_t exclamationPos = token.find('!');
    if (exclamationPos != std::string::npos) {
      size_t atPos = token.find('@');
      if (atPos == std::string::npos) {
        throw IRCComplianceException("Missing hostname");
      }

      nickname = token.substr(1, exclamationPos - 1);
      username = token.substr(exclamationPos + 1, atPos - exclamationPos - 1);
      hostname = token.substr(atPos + 1);
    } else {
      nickname = token.substr(1);
    }
    iss >> token;
  }

  if (token.empty()) {
    throw IRCComplianceException("Missing command");
  }
  command = token;

  while (iss >> token) {
    if (token[0] == ':') {
      std::string trailingParam = token.substr(1);
      if (getline(iss, token, '\r')) {
        trailingParam += token;
      }
      params.push_back(trailingParam);
      break;
    }

    params.push_back(token);
  }
}

Message::Message(const Message &rhs)
    : nickname(rhs.nickname), username(rhs.username), hostname(rhs.hostname),
      command(rhs.command), params(rhs.params) {}

Message &Message::operator=(const Message &rhs) {
  if (this != &rhs) {
    nickname = rhs.nickname;
    username = rhs.username;
    hostname = rhs.hostname;
    command = rhs.command;
    params = rhs.params;
  }
  return *this;
}

Message::~Message() {}

std::string Message::serialize() const {
  std::string serialized;

  if (!nickname.empty()) {
    serialized += ":" + nickname;
    if (!username.empty() && !hostname.empty()) {
      serialized += "!" + username;
      serialized += "@" + hostname;
    }
    serialized += " ";
  }

  serialized += command;

  if (params.size() > 0) {
    for (size_t i = 0; i < params.size() - 1; i++) {
      serialized += " " + params[i];
    }
    serialized += " :" + params[params.size() - 1];
  }

  return serialized;
}
