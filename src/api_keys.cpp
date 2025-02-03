#include "include/api_keys.hpp"
#include "include/protocol.hpp"
#include <memory>

std::unique_ptr<Abstract_Api_Key> api_key_factory(const int key,
                                                  int client_fd) noexcept {
  return std::make_unique<Abstract_Api_Key>(Api_version(client_fd));
};

Api_version::Api_version(int client_fd) : Abstract_Api_Key(client_fd){};

void Api_version::Api_version::execute(const Request &request) {
  auto size{serialize(static_cast<int32_t>(4))};
  auto id{serialize(static_cast<int32_t>(1234))};
}
