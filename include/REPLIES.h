#pragma once

#define RPL_WELCOME(nickname)                                                  \
  "001 " + nickname + " :Welcome to the Internet Relay Network "

#define ERR_NOSUCHCHANNEL(nickname, channelName)                               \
  "403 " + nickname + " " + channelName + " :No such channel"

#define ERR_NOTONCHANNEL(nickname, channelName)                                \
  "442 " + nickname + " " + channelName + " :You're not on that channel"

#define ERR_BADCHANNELKEY(nickname, channelName)                               \
  "475 " + nickname + " " + channelName + " :Cannot join channel (+k)"
