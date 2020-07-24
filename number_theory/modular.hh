#ifndef __CB_MODULAR
#define __CB_MODULAR

#include <cstdint>
#include <iostream>

#include "nt.hh"

namespace codebook {

using namespace std;

// MOD needs to be prime for inverse and division to work
// if you are only going to be using addition/multiplication
// then MOD doesn't need to be prime
template <int64_t MOD> struct modular {
  int64_t n;

  constexpr modular() : n(0) {}
  constexpr modular(const int64_t &m) : n(m) {
    n %= MOD;
    if (n < 0)
      n += MOD;
  }
  constexpr modular(const modular &m) : n(m.n) {
    n %= MOD;
    if (n < 0)
      n += MOD;
  }

  modular &operator=(const modular &m) {
    n = m.n % MOD;
    if (n < 0)
      n += MOD;
    return *this;
  }

  modular &operator+=(const modular &m) {
    n = (n + m.n) % MOD;
    if (n < 0)
      n += MOD;
    return *this;
  }
  modular &operator-=(const modular &m) {
    n = (n - m.n) % MOD;
    if (n < 0)
      n += MOD;
    return *this;
  }
  modular &operator*=(const modular &m) {
    n = (n * m.n) % MOD;
    if (n < 0)
      n += MOD;
    return *this;
  }

  modular operator+(const modular &m) const {
    modular<MOD> nn(n);
    nn += m;
    return nn;
  }
  modular operator-(const modular &m) const {
    modular<MOD> nn(n);
    nn -= m;
    return nn;
  }
  modular operator*(const modular &m) const {
    modular<MOD> nn(n);
    nn *= m;
    return nn;
  }

  modular inverse() const { return mpow(*this, MOD - 2); }

  modular &operator/=(const modular &m) {
    *this *= m.inverse();
    return *this;
  }
  modular operator/(const modular &m) const {
    modular<MOD> nn(n);
    nn /= m;
    return nn;
  }

  bool operator==(const modular &t) const { return n == t.n; }
  bool operator!=(const modular &t) const { return n != t.n; }

  operator int64_t() const { return n; }
};
template <long long T> istream &operator>>(istream &in, modular<T> &x) {
  return in >> x.n;
}
template <long long T> ostream &operator<<(ostream &out, const modular<T> &x) {
  return out << x.n;
}

} // namespace codebook

#endif // __CB_MODULAR
