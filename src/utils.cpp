inline void getSubArray(int *arr, int start, int end, int *subArr) {
  int j = 0;
  for (int i = start; i <= end; i++) {
    subArr[j++] = arr[i];
  }
}
