#ifndef __CB_FFT_
#define __CB_FFT_

#include <cmath>
#include <complex>
#include <vector>

namespace codebook {

using namespace std;

template<typename S>
void fft(vector<S> &a, size_t n, S root, size_t root_pw) {
  // first calculate the bit reverse permutation
  for (size_t i = 1, j = 0; i < n; i++) {
    size_t bit = n >> 1;
    for (; j & bit; bit >>= 1)
      j ^= bit;
    j ^= bit;
    if (i < j)
      swap(a[i], a[j]);
  }
  // we are going down the recurrence tree
  // we need log2(n) iterations, done using len
  for (size_t len = 2; len <= n; len <<= 1) {
    S wlen = root;
    for (size_t i = len; i < root_pw; i <<= 1)
      // we need root^(n//len) as the calculation variable
      wlen *= wlen;
    // convolving and going up the recurrence tree
    for (size_t i = 0; i < n; i += len) {
      S w = 1;
      for (size_t j = 0; j < len / 2; j++) {
        int u = a[i + j], v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;;
        w *= wlen;
      }
    }
  }
}

} // namespace codebook

#endif // __CB_FFT_
