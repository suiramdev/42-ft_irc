#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <string>

void privmsgCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("411 ERR_NORECIPIENT :No recipient given (PRIVMSG)");
    return;
  }

  if (params.size() < 2) {
    sender.send("412 ERR_NOTEXTTOSEND :No text to send");
    return;
  }

  if (params[0][0] == '#') {
    Channel *channel = sender.server().getChannel(params[0].substr(1));
    if (!channel) {
      sender.send("401 ERR_NOSUCHNICK " + params[0] + " :No such nick/channel");
      return;
    }

    if (!channel->hasClient(sender)) {
      sender.send("404 ERR_CANNOTSENDTOCHAN " + params[0] +
                  " :Cannot send to channel");
      return;
    }

    channel->send("PRIVMSG " + params[0] + " :" + params[1], sender);
  } else {
    Client *client = sender.server().getClient(params[0]);

    if (!client) {
      sender.send("401 ERR_NOSUCHNICK " + params[0] + " :No such nick/channel");
      return;
    }

    client->send("PRIVMSG " + params[0] + " :" + params[1]);
  }

  return;
}
