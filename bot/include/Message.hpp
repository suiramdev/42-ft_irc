#pragma once

#include <string>
#include <vector>

class IRCComplianceException : public std::exception {
private:
  std::string _msg;

public:
  IRCComplianceException(std::string message) throw()
      : _msg("IRC compliance: " + message) {}
  virtual ~IRCComplianceException() throw() {}
  virtual const char *what() const throw() { return _msg.c_str(); }
};

class Message {
private:
  void extractSource(const std::string &rawSource);
  void extractCommand(const std::string &rawCommand);
  void extractParams(const std::string &rawParams);

public:
  std::string nickname;
  std::string username;
  std::string hostname;
  std::string command;
  std::vector<std::string> params;

  Message(const std::string &command, const std::vector<std::string> &params,
          const std::string &nickname = "", const std::string &username = "",
          const std::string &hostname = "");
  Message(const std::string &rawMessage);
  Message(const Message &rhs);
  Message &operator=(const Message &rhs);
  ~Message();

  std::string serialize() const;
};
