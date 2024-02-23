#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

void partCommand(const std::vector<std::string> params, Client &sender) {
  std::istringstream channelsNameStream(params[0]);
  std::string channelName;
  while (std::getline(channelsNameStream, channelName, ',')) {
    if (params.size() > 1) {
      sender.partChannel(channelName, params[1]);
    } else {
      sender.partChannel(channelName);
    }
  }
}
