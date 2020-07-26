#ifndef __CB_MODULAR
#define __CB_MODULAR

#include <cstdint>
#include <iostream>

namespace codebook {

using namespace std;

template <typename T> inline T mpow(T a, long long n) {
  T ans = 1;
  while (n) {
    if (n % 2 == 1)
      ans *= a;
    n >>= 1;
    a *= a;
  }
  return ans;
}

template <typename T> inline T mpow(T a, long long n, T MOD) {
  T ans = 1;
  while (n) {
    if (n % 2 == 1)
      ans = (ans * a) % MOD;
    n >>= 1;
    a = (a * a) % MOD;
  }
  ans = (ans + MOD) % MOD;
  return ans;
}

template <typename T> inline T inverse(T a, T n) {
  T u = 0, v = 1;
  while (a != 0) {
    T t = n / a;
    n -= t * a;
    swap(a, n);
    u -= t * v;
    swap(u, v);
  }
  assert(n == 1);
  return u;
}

// MOD needs to be prime for inverse and division to work
// if you are only going to be using addition/multiplication
// then MOD doesn't need to be prime
template <int64_t MOD> struct modular {
  int64_t n;

  constexpr modular() : n(0) {}

  constexpr modular(const modular &m) : n(m.n) {}

  template <typename X> constexpr modular(const X &m) : n(m) {
    n %= MOD;
    if (n < 0)
      n += MOD;
  }

  modular &operator=(const modular &m) {
    n = m.n;
    return *this;
  }

  modular &operator+=(const modular &m) {
    n += m.n;
    n %= MOD;
    return *this;
  }
  modular &operator-=(const modular &m) {
    n -= m.n;
    n %= MOD;
    if (n < 0)
      n += MOD;
    return *this;
  }
  modular &operator*=(const modular &m) {
    n *= m.n;
    n %= MOD;
    return *this;
  }

  modular operator+(const modular &m) const { return modular(*this) += m; }
  modular operator-(const modular &m) const { return modular(*this) -= m; }
  modular operator*(const modular &m) const { return modular(*this) *= m; }

  modular inverse() const { return codebook::inverse(n, MOD); }

  modular &operator/=(const modular &m) {
    *this *= m.inverse();
    return *this;
  }
  modular operator/(const modular &m) const { return modular(*this) /= m; }

  modular &operator%=(const modular &m) {
    n %= m.n;
    return *this;
  }
  modular operator%(const modular &m) { return modular(*this) %= m; }

  bool operator==(const modular &t) const { return n == t.n; }
  bool operator!=(const modular &t) const { return n != t.n; }
  bool operator<(const modular &t) const { return n < t.n; }
  bool operator>(const modular &t) const { return n > t.n; }
  bool operator<=(const modular &t) const { return n <= t.n; }
  bool operator>=(const modular &t) const { return n >= t.n; }

  modular &operator++() {
    *this += 1;
    return *this;
  }
  modular &operator--() {
    *this -= 1;
    return *this;
  }

  operator int64_t() const { return n; }

  friend istream &operator>>(istream &in, modular &x) { return in >> x.n; }
  friend ostream &operator<<(ostream &out, const modular &x) {
    return out << x.n;
  }
};

} // namespace codebook

#endif // __CB_MODULAR
