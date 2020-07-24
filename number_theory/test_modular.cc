#include <bits/stdc++.h>

#include "modular.hh"

using namespace std;
using namespace codebook;

typedef modular<11> base;

int main(){
  base k(10), l(20);
  k += l;
  assert(k == (base)8);
  base g = k + l;
  assert(g == (base)6);
  g += 5;
  assert(g == (base)0);
  base h = 10;
  base i = h.inverse();
  assert(i == h);
  base j = h * i;
  assert(j == (base)1);
  j = 20;
  j /= 9;
  assert(j == (base)1);
}
