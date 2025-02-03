

#include "include/buffer.hpp"
#include "include/primitives.hpp"
#include "include/protocol.hpp"
#include "include/utils.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

R_header parse_headers(std::vector<std::byte> data) {

  int16_t api_key{de_serialize<int16_t>(slice(0, 3, data))};
  int16_t request_api_version{de_serialize<int16_t>(slice(3, 5, data))};
  int32_t correlation_id{de_serialize<int32_t>(slice(5, 9, data))};
  Nullable_string client_id;
  Compact_arr<size_t> tag_buffer;

  return R_header(api_key, request_api_version, correlation_id, client_id,
                  tag_buffer);
};
