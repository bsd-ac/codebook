#ifndef __CB_POLY
#define __CB_POLY

// CREDIT: a lot of this was borrowed from
// https://github.com/e-maxx-eng/e-maxx-eng-aux/blob/master/src/polynomial.cpp

#include <bits/stdc++.h>

#include "fft.hh"

namespace codebook {

using namespace std;

// TODO: ideally here we should have
//       root to be of type T but this is not allowed
//       int c++17 so we need to set it to long long/int64_t
template <typename T, int64_t rootx, size_t root_pw> struct poly {

  vector<T> a;
  static constexpr size_t CUTOFF = 128;
  const T root = (T)rootx;

  void normalize() {
    while (!a.empty() && a.back() == (T)0)
      a.pop_back();
  }

  poly() {}
  poly(T a0) : a{a0} { normalize(); }
  poly(const vector<T> &t) : a(t) { normalize(); }
  poly(const poly &t) : a(t.a) { normalize(); }

  poly operator=(const poly &t) {
    a = t.a;
    normalize();
    return *this;
  }

  size_t size() const { return a.size(); }

  T operator[](const size_t k) const { return a[k]; }

  bool operator == (const poly& p){
    if(a.size() != p.size())
      return false;
    for(int i = 0; i < a.size(); i++)
      if(a[i] != p[i])
        return false;
    return true;
  }

  bool operator != (const poly& p){
    return !(p == (*this));
  }

  poly &operator+=(const poly &t) {
    a.resize(max(a.size(), t.size()));
    for (size_t i = 0; i < t.size(); i++)
      a[i] += t[i];
    normalize();
    return *this;
  }

  poly &operator-=(const poly &t) {
    a.resize(max(a.size(), t.size()));
    for (size_t i = 0; i < t.size(); i++)
      a[i] -= t[i];
    normalize();
    return *this;
  }

  poly operator+(const poly &t) const { return poly(*this) += t; }
  poly operator-(const poly &t) const { return poly(*this) -= t; }

  poly slow_multiply(const poly &t) const {
    vector<T> c;
    c.resize(a.size() + t.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
      for (size_t j = 0; j < t.size(); j++)
        c[i + j] += a[i] * t[j];
    return c;
  }

  poly fast_multiply(const poly &t) const {
    vector<T> c(t.a), d(a);
    size_t n = 1;
    while (n < c.size() + d.size() - 1)
      n *= 2;
    c.resize(n, 0);
    d.resize(n, 0);
    fft(c, n, root, root_pw);
    fft(d, n, root, root_pw);
    for (size_t i = 0; i < n; i++)
      c[i] *= d[i];
    T root_inv = 1;
    root_inv /= root;
    fft(c, n, root_inv, root_pw);
    for (size_t i = 0; i < n; i++)
      c[i] /= n;
    return c;
  }

  poly &operator*=(const poly &p) {
    if (min(a.size(), p.size()) < CUTOFF)
      a = slow_multiply(p).a;
    else {
      a = fast_multiply(p).a;
    }
    return *this;
  }
  poly operator*(const poly &p) const { return poly(*this) *= p; }

  // shift operators
  // equivalent to multiplying/dividing by x^k
  poly &operator<<=(const size_t k) {
    std::reverse(a.begin(), a.end());
    a.resize(a.size() + k, 0);
    std::reverse(a.begin(), a.end());
    return *this;
  }
  poly &operator>>=(const size_t k) {
    std::reverse(a.begin(), a.end());
    a.resize(max(0, (int)a.size() - (int)k));
    std::reverse(a.begin(), a.end());
    return *this;
  }
  poly operator<<(const size_t k) const { return poly(*this) <<= k; }
  poly operator>>(const size_t k) const { return poly(*this) >>= k; }

  bool is_zero() const { return a.size() == 0; }

  poly substr(size_t l, size_t r) const {
    l = min(l, a.size());
    r = min(r, a.size());
    return vector<T>(a.begin() + l, a.begin() + r);
  }

  poly mod_xk(size_t k) const { // get same polynomial mod x^k
    k = min(k, a.size());
    return vector<T>(a.begin(), a.begin() + k);
  }

  poly inverse(size_t n) const {
    assert(!is_zero());
    T a0_inv = 1;
    a0_inv /= a[0];
    poly inv(a0_inv);
    size_t l = 1;
    while (l < n) {
      poly c = (inv * mod_xk(2 * l)).substr(l, 2 * l);
      inv -= ((inv * c).mod_xk(l)) << l;
      l *= 2;
    }
    return inv.mod_xk(n);
  }

  poly reverse(size_t n, bool rev = 0) const {
    poly res(*this);
    if (rev) { // If rev = 1 then tail goes to head
      res.a.resize(max(n, res.a.size()));
    }
    std::reverse(res.a.begin(), res.a.end());
    return res.mod_xk(n);
  }

  pair<poly, poly> slow_divide(const poly &p) const {
    vector<T> q, r(a);
    while (p.size() <= r.size()) {
      T coeff = r.back() / p.a.back();
      q.push_back(coeff);
      if (q.back() != (T)0)
        for (size_t i = 0; i < p.a.size(); i++)
          r[r.size() - 1 - i] -= p.a[p.a.size() - 1 - i] * coeff;
      r.pop_back();
    }
    std::reverse(q.begin(), q.end());
    return {q, r};
  }

  pair<poly, poly> fast_divide(const poly &p) const { 
    if (p.size() > a.size())
      return { (poly)0, *this};
    int d = a.size() - p.size();
    poly q = (reverse(d + 1) * p.reverse(d + 1).inverse(d + 1)).mod_xk(d + 1).reverse(d + 1, 1);
    return {q, *this - (p*q)};
  }

  pair<poly, poly> divide(const poly &p) const {
    int d = min(p.size(), a.size() - p.size());
    if(d < CUTOFF)
      return slow_divide(p);
    else
      return fast_divide(p);
  }

  poly &operator /= (const poly& p){
    *this = divide(p).first;
    return *this;
  }
  poly &operator %= (const poly& p){
    *this = divide(p).second;
    return *this;
  }

  poly operator /(const poly &p) { return poly(*this) /= p; }
  poly operator %(const poly &p) { return poly(*this) %= p; }
};

// for now root is going to be int64_t
template <typename T, int64_t root, size_t root_pw>
ostream &operator<<(ostream &out, const poly<T, root, root_pw> &p) {
  out << "{ ";
  if (p.a.size() > 0)
    cout << p.a[0];
  for (size_t i = 1; i < p.size(); i++)
    out << ", " << p.a[i];
  out << " }";
  return out;
}

} // namespace codebook

#endif // __CB_POLY
