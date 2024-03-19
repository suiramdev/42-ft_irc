#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void kickCommand(const std::vector<std::string> params, Client &sender) {
  std::string channelName = params[0];

  if (channelName[0] != '#') {
    sender.send(ERR_BADCHANMASK(params[0]));
    return;
  }

  Channel *channel = sender.server().getChannel(params[0]);
  if (!channel) {
    sender.send(ERR_NOSUCHCHANNEL(sender.nickname, params[0]));
    return;
  }

  if (!channel->hasMember(sender)) {
    sender.send(ERR_NOTONCHANNEL(sender.nickname, params[0]));
    return;
  }

  if (!channel->isOperator(sender)) {
    sender.send(ERR_CHANOPRIVSNEEDED(sender.nickname, params[0]));
    return;
  }

  Client *target = channel->getMember(params[1]);

  if (!target) {
    sender.send(
        ERR_USERNOTINCHANNEL(sender.nickname, target->nickname, params[0]));
    return;
  }

  target->kickChannel(params[0]);

  channel->send(
      Message("KICK", params, sender.nickname, sender.username, sender.hostname)
          .serialize());
}
