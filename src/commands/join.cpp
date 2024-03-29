#include "Channel.hpp"
#include "Client.hpp"
#include "REPLIES.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

void joinCommand(const std::vector<std::string> params, Client &sender) {
  if (params[0] == "0") {
    for (std::map<std::string, Channel *>::iterator it =
             sender.channels().begin();
         it != sender.channels().end(); ++it) {
      sender.partChannel(it->first);
    }
    return;
  }

  std::istringstream channelsNameStream(params[0]);
  std::istringstream channelsKeyStream(params.size() > 1 ? params[1] : "");
  std::string channelName;
  std::string channelKey;
  while (std::getline(channelsNameStream, channelName, ',')) {
    if (channelName[0] != '#') {
      sender.send(ERR_BADCHANMASK(channelName));
      continue;
    }

    if (!std::getline(channelsKeyStream, channelKey, ',')) {
      sender.joinChannel(channelName);
    } else {
      sender.joinChannel(channelName, channelKey);
    }
  }
}
