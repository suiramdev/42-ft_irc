#pragma once

#include <exception>
#include <string>

class SocketException : public std::exception {
private:
  std::string _msg;

public:
  SocketException(std::string message) throw() : _msg("socket: " + message) {}
  virtual ~SocketException() throw() {}
  virtual const char *what() const throw() { return _msg.c_str(); }
};

class Socket {
private:
  int _fd;
  struct addrinfo *_res;

public:
  Socket();
  ~Socket();

  const int &getFd() { return _fd; }

  /**
   * @brief Check if the socket is connected to a server
   *
   * @return true if the socket is connected, false otherwise
   */
  bool isConnected() { return _fd != -1; }

  /**
   * @brief Connect to a server
   *
   * @param hostname The hostname of the server
   * @param port The port to connect to
   * @throws SocketException if the connection fails
   */
  void connect(const char *hostname, const char *port);
};
