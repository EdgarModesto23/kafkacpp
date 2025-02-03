#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Abstract_Api_Key {
protected:
  int client_fd;
  std::unique_ptr<std::vector<uint8_t>> request;
  std::vector<uint8_t> get_throttle_time_ms();
  std::vector<uint8_t> get_tagged_fields();

public:
  Abstract_Api_Key(int client_fd, std::unique_ptr<std::vector<uint8_t>> data)
      : client_fd(client_fd), request(std::move(data)){};
};

std::unique_ptr<Abstract_Api_Key> api_key_factory(int key) noexcept;

class Api_version : public Abstract_Api_Key {
public:
  void execute();
  std::vector<uint8_t> get_supported_versions();
  Api_version(int client_fd, std::unique_ptr<std::vector<uint8_t>> data)
      : Abstract_Api_Key(client_fd, std::move(data)) {}
};
