#include "Channel.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

void modeCommand(const std::vector<std::string> params, Client &sender) {
  Channel *channel = sender.server().getChannel(params[0]);
  if (!channel) {
    sender.send(ERR_NOSUCHCHANNEL(sender.nickname, params[0]));
    return;
  }

  if (params.size() == 1) {
    // TODO: Send RPL_CHANNELMODEIS followed by RPL_CREATIONTIME
    return;
  }

  if (!channel->isOperator(sender)) {
    sender.send(ERR_CHANOPRIVSNEEDED(sender.nickname, params[0]));
    return;
  }

  bool add = params[1][0] == '+';
  channel->modes |= INVITE_MODE;
  for (std::string::size_type i = 1; i < params[1].length(); ++i) {
    switch (params[1][i]) {
    case 'i': // Allow only invited members to join
      if (add) {
        channel->modes |= INVITE_MODE;
      } else {
        channel->modes &= ~INVITE_MODE;
      }
      break;
    case 'k': // Set the channel key
      if (add) {
        if (params.size() < 3) {
          sender.send(ERR_NEEDMOREPARAMS(sender.nickname, "MODE"));
          return;
        }

        channel->modes |= KEY_MODE;
        channel->key = params[2];
      } else {
        channel->modes &= ~KEY_MODE;
      }
      break;
    case 'l': // Give the channel a member limit
      if (add) {
        channel->modes |= LIMIT_MODE;
        if (params.size() > 2) {
          std::istringstream ss(params[2]);
          ss >> channel->maxMembers;
        }
      } else {
        channel->modes &= ~LIMIT_MODE;
      }
      break;
    case 't': // Allow anyone to set the topic
      if (add) {
        channel->modes |= TOPIC_MODE;
      } else {
        channel->modes &= ~TOPIC_MODE;
      }
      break;
    }
  }

  channel->send(":" + sender.nickname + "!" + sender.username + "@" +
                sender.hostname + " MODE " + params[0] + " " + params[1]);
}
