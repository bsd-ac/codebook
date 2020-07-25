#include <bits/stdc++.h>

#include "modular.hh"
#include "polynomial.hh"

using namespace std;
using namespace codebook;

#define base modular<998244353>

constexpr base root(15311432);
constexpr size_t root_pw = (1<<23);

typedef poly<base, 15311432, root_pw> xpoly;

int main(){

  vector<base> a;
  for(int i = 0; i < 10000; i++)
    a.push_back(1);
  xpoly p(a), q(a);
  xpoly pq = p * q;
  cout << pq / p << endl << pq / q << endl << ((pq/p) == q) << endl << ((pq/q) == p) << endl;
  /*
  auto qr = p.slow_divide(q);
  cout << qr.second.size() << endl;
  cout << qr.first << endl << qr.second << endl;
  vector<base> x({2,2,4,8}), y({1,2});
  xpoly s(x), t(y);
  auto ab = s.slow_divide(t);
  cout << ab.first << endl << ab.second << endl;

  p += q;
  cout << p<< endl;
  p -= q;
  cout << p<< endl;
  p *= q;
  cout << p << endl;
  */
}
