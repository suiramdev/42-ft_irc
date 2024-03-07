#include "Channel.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <ctime>
#include <sstream>

void topicCommand(const std::vector<std::string> params, Client &sender) {
  Channel *channel = sender.server().getChannel(params[0]);
  if (!channel) {
    sender.send(ERR_NOSUCHCHANNEL(sender.nickname, params[0]));
    return;
  }

  if (!channel->hasMember(sender)) {
    sender.send(ERR_NOTONCHANNEL(sender.nickname, params[0]));
    return;
  }

  if (channel->modes & ~TOPIC_MODE && !channel->isOperator(sender)) {
    sender.send(ERR_CHANOPRIVSNEEDED(sender.nickname, params[0]));
    return;
  }

  if (params.size() == 1) {
    if (channel->topic.empty()) {
      sender.send(RPL_NOTOPIC(sender.nickname, params[0]));
    } else {
      sender.send(RPL_TOPIC(sender.nickname, params[0], channel->topic));
      std::stringstream ss;
      ss << channel->topicSetTime;
      sender.send(RPL_TOPICWHOTIME(sender.nickname, params[0], channel->topic,
                                   channel->topicSetter->nickname, ss.str()));
    }
  } else {
    if (!channel->isOperator(sender)) {
      sender.send(ERR_CHANOPRIVSNEEDED(sender.nickname, params[0]));
      return;
    }
    channel->topic = params[1];
    channel->topicSetter = &sender;
    channel->topicSetTime = time(NULL);
    channel->send(":" + sender.nickname + "!" + sender.username + "@" +
                  sender.hostname + " TOPIC " + params[0] + " :" + params[1]);
  }
}
