#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <string>

void inviteCommand(const std::vector<std::string> params, Client &sender) {
  Channel *channel = sender.server().getChannel(params[1]);
  if (!channel) {
    sender.send(ERR_NOSUCHCHANNEL(sender.nickname, params[1]));
    return;
  }

  if (!channel->hasMember(sender)) {
    sender.send(ERR_NOTONCHANNEL(sender.nickname, params[1]));
    return;
  }

  if (!channel->isOperator(sender)) {
    sender.send(ERR_CHANOPRIVSNEEDED(sender.nickname, params[1]));
    return;
  }

  Client *target = sender.server().getClient(params[0]);
  if (!target) {
    return; // TODO: Note sure about this
  }

  if (channel->hasMember(*target)) {
    sender.send(ERR_USERONCHANNEL(sender.nickname, params[0], params[1]));
    return;
  }

  sender.send(RPL_INVITING(sender.nickname, params[0], params[1]));

  target->send(Message("INVITE", params, sender.nickname, sender.username,
                       sender.hostname)
                   .serialize());

  channel->inviteMember(*target);
}
