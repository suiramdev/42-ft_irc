#include "Client.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void nickCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send(ERR_NONICKNAMEGIVEN(sender.nickname));
    return;
  }

  if (params[0].find('#') != std::string::npos ||
      params[0].find(':') != std::string::npos ||
      params[0].find(' ') != std::string::npos) {
    sender.send(ERR_ERRONEUSNICKNAME(sender.nickname, params[0]));
    return;
  }

  if (sender.server().getClient(params[0])) {
    sender.send(ERR_NICKNAMEINUSE(sender.nickname, params[0]));
    return;
  }

  if (!sender.nickname.empty()) {
    sender.server().broadcast(Message("NICK", {params[0]}, sender.nickname,
                                      sender.username, sender.hostname)
                                  .serialize());
  }

  sender.nickname = params[0];
}
