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
      sender.joinChannel(channelName);
    } else {
      sender.joinChannel(channelName, channelKey);
    }
  }

  return;
}
