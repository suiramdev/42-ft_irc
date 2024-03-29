#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
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

  bool add = true;
  for (std::string::size_type i = 1; i < params[1].length(); ++i) {
    switch (params[1][i]) {
    case '+':
    case '-':
      add = params[1][i] == '+';
      break;
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
    case 'o': {
      Client *target = channel->getMember(params[2]);
      if (!target) {
        sender.send(
            ERR_USERNOTINCHANNEL(sender.nickname, params[2], params[0]));
        return;
      }

      channel->setOperator(*target, add);
      break;
    }
    default:
      sender.send(ERR_UNKNOWNMODE(sender.nickname, params[1][i]));
      break;
    }

    std::vector<std::string> sendParams = params;
    if (params.size() > 2) {
      sendParams.erase(sendParams.begin() + 2);
    }
    channel->send(Message("MODE", sendParams, sender.nickname, sender.username,
                          sender.hostname)
                      .serialize());
  }
}
