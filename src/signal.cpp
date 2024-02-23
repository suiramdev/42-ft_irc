#include "signals.hpp"
#include "utils/Logger.hpp"
#include <sstream>

int receivedSignal = -1;

void signalHandler(int signum) {
  receivedSignal = signum;

  std::stringstream ss;
  ss << "Received signal " << signum;
  Logger::info(ss.str());
}
