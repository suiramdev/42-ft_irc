#include "Channel.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void privmsgCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send(ERR_NORECIPIENT(sender.nickname));
    return;
  }

  if (params.size() < 2) {
    sender.send(ERR_NOTEXTTOSEND(sender.nickname));
    return;
  }

  if (params[0][0] == '#') {
    Channel *channel = sender.server().getChannel(params[0]);
    if (!channel) {
      sender.send(ERR_NOSUCHNICK(sender.nickname, params[0]));
      return;
    }

    if (!channel->hasMember(sender)) {
      sender.send(ERR_CANNOTSENDTOCHAN(sender.nickname, params[0]));
      return;
    }

    channel->send(":" + sender.nickname + "!" + sender.username + "@" +
                      sender.hostname + " PRIVMSG " + params[0] + " :" +
                      params[1],
                  sender);
  } else {
    Client *client = sender.server().getClient(params[0]);

    if (!client) {
      sender.send(ERR_NOSUCHNICK(sender.nickname, params[0]));
      return;
    }

    client->send(":" + sender.nickname + "!" + sender.username + "@" +
                 sender.hostname + " PRIVMSG " + params[0] + " :" + params[1]);
  }
}
