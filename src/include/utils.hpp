#pragma once

#include <vector>
template <typename T>
inline void getSubArray(T *arr, int start, int end, T *subArr) {
  int j = 0;
  for (int i = start; i <= end; i++) {
    subArr[j++] = arr[i];
  }
}

template <typename T>
std::vector<T> slice(int begin, int end, const std::vector<T> &data) {
  std::vector<T> sliced_vector;
  for (auto i = begin; i < end; i++) {
    sliced_vector.push_back(data[i]);
  }
  return sliced_vector;
};
