#include "include/api_keys.hpp"
#include "include/protocol.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
#include <unordered_set>
#include <vector>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/parse.h>

#include "include/utils.hpp"
#include "yaml-cpp/yaml.h"

std::vector<uint8_t> Abstract_Api_Key::get_throttle_time_ms() {
  return std::vector<uint8_t>{0};
};

std::vector<uint8_t> Abstract_Api_Key::get_tagged_fields() {
  return std::vector<uint8_t>{0};
};

std::unique_ptr<Abstract_Api_Key>
api_key_factory(const int key, int client_fd,
                std::unique_ptr<std::vector<uint8_t>> &data) noexcept {
  return std::make_unique<Api_version>(client_fd, std::move(data));
};

std::vector<uint8_t> Api_version::get_supported_versions() {

  std::vector<uint8_t> supported_versions;

  YAML::Node file = YAML::LoadFile("api-keys.yaml");
  if (file["Supported-api-keys"]) {
    auto num_api_keys =
        serialize<int8_t>(file["Supported-api-keys"].size() + 1);
    supported_versions.insert(supported_versions.end(), num_api_keys.begin(),
                              num_api_keys.end());
    std::cout << file["Supported-api-keys"] << '\n';
    for (auto key : file["Supported-api-keys"]) {
      for (auto pair : key) {
        auto key = serialize<int16_t>(pair.first.as<uint16_t>());
        auto min_value = serialize<int16_t>(pair.second[0].as<uint16_t>());
        auto max_value = serialize<int16_t>(pair.second[1].as<uint16_t>());
        auto tag_buffer = serialize<int8_t>(0);

        auto api_key_data =
            join_vectors<uint16_t>(key, min_value, max_value, tag_buffer);

        supported_versions.insert(supported_versions.end(),
                                  api_key_data->begin(), api_key_data->end());
      }
    }
    return supported_versions;
  } else {
    throw YAML::BadFile("API keys file not found");
    return supported_versions;
  }
};

void Api_version::Api_version::execute() {

  auto headers = slice(8, 12, *request);

  auto correlation_id{de_serialize<uint32_t>(*request, 8, 4)};

  auto error_code{serialize<int16_t>(0)};

  auto throttle_ms{serialize<int32_t>(0)};

  auto tag_buffer{serialize<uint8_t>(0)};

  try {
    auto api_keys{get_supported_versions()};

    auto supported_range = SUPPORTED_API_KEY_VERSIONS.find("ApiVersion");

    auto is_supported = is_request_supported(
        supported_range->second[0], supported_range->second[1],
        de_serialize<int16_t>(*request, 6, 2));

    if (!is_supported) {
      error_code = serialize<int16_t>(35);
      auto res_size = get_vectors_total_size(headers, error_code);

      auto response = join_vectors<uint8_t>(serialize<int32_t>(res_size),
                                            headers, error_code);

      ssize_t bytesSent =
          send(client_fd, response->data(), response->size(), 0);
      if (bytesSent == -1) {
        std::cerr << "Failed to send data to client:" << correlation_id
                  << std::endl;
        close(client_fd);
      }
      return;
    }

    auto body =
        join_vectors<uint8_t>(error_code, api_keys, throttle_ms, tag_buffer);

    auto res_size = get_vectors_total_size(headers, *body);

    auto response =
        join_vectors<uint8_t>(serialize<int32_t>(res_size), headers, *body);

    ssize_t bytesSent = send(client_fd, response->data(), response->size(), 0);
    if (bytesSent == -1) {
      std::cerr << "Failed to send data to client:" << correlation_id
                << std::endl;
      close(client_fd);
    }

  } catch (YAML::BadFile) {
    error_code = serialize<int16_t>(-1);
    ssize_t bytesSent =
        send(client_fd, error_code.data(), error_code.size(), 0);
    if (bytesSent == -1) {
      std::cerr << "Failed to send data to client:" << correlation_id
                << std::endl;
      close(client_fd);
    }
  }
}
