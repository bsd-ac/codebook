#ifndef _NT_
#define _NT_

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "modular.hh"

namespace codebook {

using namespace std;

template <typename T> inline T gcd(T a, T b) {
  while( b != 0){
    a %= b;
    swap(a, b);
  }
  return a;
}

// deterministic version of miller rabin test
bool miller_rabin(long long n){
  if(n % 2 == 0)
    return false;
  vector<long long> witness = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };
  int pow2 = 1;
  while(((n-1) & (1<<pow2)) == 0)
    pow2++;
  long long d = (n - 1)/(1<<pow2);
  for(long long w: witness){
    long long wd = mpow(w, d, n);
    if ((wd == 1) || (wd == n-1))
      continue;
    bool is_composite = true;
    for(int i = 0; i < pow2; i++){
      wd = (wd * wd) % n;
      if (wd == n-1)
        is_composite = false;
    }
    if (is_composite)
      return false;
  }
  return true;
}

/*
 *
 * TODO: THINGS ARE STILL LEFT HERE
 *
 *
const int maxn = (int)(1e5) + 10;
const int e97 = (1e9) + 7;
const int e99 = (1e9) + 9;

typedef modular<e99> base;

base choose[maxn][maxn] = {0};
template <typename T> void chooseINIT() {
  for (int i = 0; i < maxn; i++)
    choose[i][0] = choose[i][i] = 1;
  for (int i = 0; i < maxn; i++)
    for (int j = 1; j < i; j++) {
      choose[i][j] = choose[i - 1][j] + choose[i - 1][j - 1];
    }
}

int spf[maxn] = {0};
void sieve() {
  spf[1] = 1;
  for (int i = 2; i * i < maxn; i++) {
    if (spf[i] == i) {
      for (int j = i * i; j < maxn; j += i)
        if (spf[j] == j)
          spf[j] = i;
    }
  }
}

set<int> facs[maxn];
set<int> factorization(int n) {
  if (facs[n].size() == 0) {
    set<int> factors;
    factors.insert(1);
    if (n == 1)
      return factors;
    set<int> recFactors = factorization(n / spf[n]);
    for (int f : recFactors) {
      factors.insert(f);
      factors.insert(f * spf[n]);
    }
    facs[n] = factors;
  }
  return facs[n];
}

map<int, int> prime_factorization(int n) {
  map<int, int> pFactors;
  if (n == 1)
    return pFactors;
  int k = n;
  while (k % spf[n] == 0) {
    k /= spf[n];
    pFactors[spf[n]]++;
  }
  map<int, int> recFactors = prime_factorization(k);
  for (auto it : recFactors)
    pFactors[it.first] = it.second;
  return pFactors;
}
*/

} // namespace codebook

#endif // _NT_
