#pragma once

#include <vector>
template <typename T> inline auto getSubArray(T *arr, int start, int end) {
  int j = 0;
  std::vector<T> subArr;
  for (int i = start; i <= end; i++) {
    subArr[j++] = arr[i];
  }
  return subArr;
}

template <typename T>
std::vector<T> slice(int begin, int end, const std::vector<T> &data) {
  std::vector<T> sliced_vector;
  for (auto i = begin; i < end; i++) {
    sliced_vector.push_back(data[i]);
  }
  return sliced_vector;
};
