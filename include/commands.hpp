#pragma once

#include <string>

class Client;

void capCommand(const std::vector<std::string> params, Client &sender);
void passCommand(const std::vector<std::string> params, Client &sender);
void nickCommand(const std::vector<std::string> params, Client &sender);
void userCommand(const std::vector<std::string> params, Client &sender);
void joinCommand(const std::vector<std::string> params, Client &sender);
void pingCommand(const std::vector<std::string> params, Client &sender);
void quitCommand(const std::vector<std::string> params, Client &sender);
void privmsgCommand(const std::vector<std::string> params, Client &sender);
