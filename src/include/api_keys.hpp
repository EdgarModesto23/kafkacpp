#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, std::array<int, 2>>
    SUPPORTED_API_KEY_VERSIONS{{"ApiVersion", std::array<int, 2>{4, 4}}};

class Abstract_Api_Key {
protected:
  int client_fd;
  std::unique_ptr<std::vector<uint8_t>> request;
  std::vector<uint8_t> get_throttle_time_ms();
  std::vector<uint8_t> get_tagged_fields();

public:
  Abstract_Api_Key(int client_fd, std::unique_ptr<std::vector<uint8_t>> data)
      : client_fd(client_fd), request(std::move(data)){};

  virtual void execute() = 0;
  virtual ~Abstract_Api_Key() = default;
};

std::unique_ptr<Abstract_Api_Key>
api_key_factory(const int key, int client_fd,
                std::unique_ptr<std::vector<uint8_t>> &data) noexcept;

class Api_version : public Abstract_Api_Key {
public:
  void execute() override;
  auto get_supported_versions(std::string key);
  std::vector<uint8_t> get_supported_versions();
  Api_version(int client_fd, std::unique_ptr<std::vector<uint8_t>> data)
      : Abstract_Api_Key(client_fd, std::move(data)) {}
};
