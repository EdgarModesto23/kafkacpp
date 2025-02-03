#include "include/api_keys.hpp"
#include "include/protocol.hpp"
#include <cstdint>
#include <memory>
#include <sys/socket.h>
#include <unordered_set>
#include <vector>

std::vector<uint8_t> Abstract_Api_Key::get_throttle_time_ms() {
  return std::vector<uint8_t>{0};
};

std::vector<uint8_t> Abstract_Api_Key::get_tagged_fields() {
  return std::vector<uint8_t>{0};
};

std::unique_ptr<Abstract_Api_Key>
api_key_factory(const int key, int client_fd,
                std::unique_ptr<std::vector<uint8_t>> &data) noexcept {
  return std::make_unique<Abstract_Api_Key>(
      Api_version(client_fd, std::move(data)));
};

std::vector<uint8_t> Api_version::get_supported_versions() {
  std::unordered_set<int> supported_versions{4};
};

void Api_version::Api_version::execute() {
  int32_t size{de_serialize<int32_t>(*request.get(), 0, 3)};

  auto request_api_key{de_serialize<int16_t>(*request.get(), 4, 2)};

  auto request_api_version{de_serialize<uint16_t>(*request.get(), 6, 2)};

  auto correlation_id{de_serialize<uint32_t>(*request.get(), 8, 4)};

  auto res_size{serialize<int32_t>(4)};

  auto res_data{serialize<int32_t>(correlation_id)};

  auto error_code{serialize<int16_t>(0)};

  std::vector<uint8_t> response;

  response.insert(response.end(), error_code.begin(), error_code.end());

  response.insert(response.end(), error_code.begin(), error_code.end());

  ssize_t bytesSent = send(client_fd, response.data(), response.size(), 0);
}
