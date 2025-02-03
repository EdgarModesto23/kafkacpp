#pragma once

#include "protocol.hpp"

class Abstract_Api_Key {
protected:
  int client_fd;

public:
  Abstract_Api_Key(int client_fd) : client_fd(client_fd) {}
};

std::unique_ptr<Abstract_Api_Key> api_key_factory(int key) noexcept;

class Api_version : public Abstract_Api_Key {
public:
  void execute(const Request &request);
  Api_version(int client_fd);
};
