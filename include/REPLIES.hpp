#pragma once

#define RPL_WELCOME(clientNickname)                                            \
  "001 " + clientNickname + " :Welcome to the Internet Relay Network "

#define ERR_NOSUCHNICK(clientNickname, nickname)                               \
  "401 " + clientNickname + " " + nickname + " :No such nick/channel"

#define ERR_NOSUCHCHANNEL(clientNickname, channelName)                         \
  "403 " + clientNickname + " " + channelName + " :No such channel"

#define ERR_CANNOTSENDTOCHAN(clientNickname, channelName)                      \
  "404 " + clientNickname + " " + channelName + " :Cannot send to channel"

#define ERR_NORECIPIENT(clientNickname)                                        \
  "411 " + clientNickname + " :No recipient given (PRIVMSG)"

#define ERR_NOTEXTTOSEND(clientNickname)                                       \
  "412 " + clientNickname + " :No text to send"

#define ERR_UNKNOWNCOMMAND(clientNickname, commandName)                        \
  "421 " + clientNickname + " " + commandName + " :Unknown command"

#define ERR_NONICKNAMEGIVEN(clientNickname)                                    \
  "431 " + clientNickname + " :No nickname given"

#define ERR_ERRONEUSNICKNAME(clientNickname, nickname)                         \
  "432 " + clientNickname + " " + nickname + " :Erroneus nickname"

#define ERR_NICKNAMEINUSE(clientNickname, nickname)                            \
  "433 " + clientNickname + " " + nickname + " :Nickname is already in use"

#define ERR_NOTONCHANNEL(clientNickname, channelName)                          \
  "442 " + clientNickname + " " + channelName + " :You're not on that channel"

#define ERR_NOTREGISTERED(clientNickname)                                      \
  "451 " + clientNickname + " :You have not registered"

#define ERR_NEEDMOREPARAMS(clientNickname, commandName)                        \
  "461 " + clientNickname + " " + commandName + " :Not enough parameters"

#define ERR_ALREADYREGISTRED(clientNickname)                                   \
  "462 " + clientNickname + " :You may not reregister"

#define ERR_PASSWDMISMATCH(clientNickname)                                     \
  "464 " + clientNickname + " :Password incorrect"

#define ERR_BADCHANNELKEY(clientNickname, channelName)                         \
  "475 " + clientNickname + " " + channelName + " :Cannot join channel (+k)"
