#pragma once

#include "primitives.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

class R_header {
private:
  size_t length;
  int16_t api_key;
  int16_t api_version;
  int32_t correlation_id;
  Nullable_string client_id;
  Compact_arr<size_t> tagged_fields;

public:
  size_t get_length() { return length; };

  void setApiKey(int16_t key) { api_key = key; }

  int16_t getApiKey() const { return api_key; }

  void setApiVersion(int16_t version) { api_version = version; }

  int16_t getApiVersion() const { return api_version; }

  void setCorrelationId(int32_t id) { correlation_id = id; }

  int32_t getCorrelationId() const { return correlation_id; }

  R_header(int16_t api_key, int16_t api_version, int32_t correlation_id,
           Nullable_string client_id, Compact_arr<size_t> tagged_fields)
      : api_key(api_key), correlation_id(correlation_id), client_id(client_id),
        tagged_fields(tagged_fields){};
};

R_header parse_headers(std::vector<std::byte> data);
