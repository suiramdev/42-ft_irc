#include "Client.hpp"
#include "Server.hpp"
#include <string>

void pingCommand(const std::vector<std::string> params, Client &sender) {
  if (params.size() < 1) {
    sender.send("409 ERR_NOORIGIN :No origin specified");
    return;
  }

  // TODO: Handle the ERR_NOORIGIN error

  /* if (params[0] != sender.server.hostname) { */
  /*   sender.send("409 ERR_NOORIGIN :No origin specified"); */
  /*   return; */
  /* } */

  if (params.size() == 2) {
    sender.send("PONG " + params[0] + " " + params[1]);
  } else {
    sender.send("PONG " + params[0]);
  }

  return;
}
