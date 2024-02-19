#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

void joinCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("461 ERR_NEEDMOREPARAMS JOIN :Not enough parameters");
    return;
  }

  if (params[0] == "0") {
    // TODO: Leave all channels
    return;
  }

  std::istringstream channelsNameStream(params[0]);
  std::istringstream channelsKeyStream(params.size() > 1 ? params[1] : "");
  std::string channelName;
  std::string channelKey;
  while (std::getline(channelsNameStream, channelName, ',')) {
    if (channelName[0] == '#') {
      channelName = channelName.substr(1);
    }

    if (!std::getline(channelsKeyStream, channelKey, ',')) {
      channelKey = "";
    }

    Channel *channel = sender.server().getChannel(channelName);
    if (!channel) {
      channel = sender.server().addChannel(channelName, channelKey);
    }

    if (!channel->key.empty() && channelKey != channel->key) {
      sender.send("475 ERR_BADCHANNELKEY " + channelName +
                  " :Cannot join channel (+k)");
      return;
    }

    channel->addClient(sender);
    if (channel->topic.empty()) {
      sender.send("331 RPL_NOTOPIC " + channelName + " :No topic is set");
    } else {
      sender.send("332 RPL_TOPIC " + channelName + " :" + channel->topic);
    }
  }

  return;
}
