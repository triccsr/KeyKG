//
// Created by triccsr on 4/1/23.
//

#include <bits/utility.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fibheap.hpp>
#include <functional>
#include <queue>
#include <random>
#include <map>
#include <utility>
#include <vector>

using namespace std;
const int inf = 0x3f3f3f3f;
const int CASE = 1000, INDEX = 200, RANGE = 10000, OPR=1000;
void single_case() {
  FibHeap<pair<int, int>> heap;
  heap.push(make_pair(10, 5));
  heap.push(make_pair(50, 2));
  heap.push(make_pair(80, 3));
  auto ii=heap.push(make_pair(40, 1));
  heap.push(make_pair(80, 4));
  heap.modify(ii,make_pair(80,1));
}

bool check_heap() {
  FibHeap<pair<int,int> >::iterator it[INDEX+1];
  int chk[INDEX+1];
  FibHeap<pair<int, int> > heap;
  random_device rd;
  mt19937 gen1(rd());
  mt19937 gen2(rd());
  mt19937 gen3(rd());
  uniform_int_distribution<int> rand_num(1, RANGE), rand_idx(1,INDEX);
  memset(chk, inf, sizeof(chk));
  for (int i = 1; i <= INDEX; ++i) {
    it[i]=nullptr;
  }
  for(int opr=1;opr<=OPR;++opr) {
    int idx = rand_idx(gen1), num = rand_num(gen2);
    chk[idx] = num;
    //printf("opration %d: modify on index %d with num %d\n",opr,idx,num);
    if (it[idx] == nullptr) {
      printf("heap:push idx:%d num:%d\n",idx,num);
      it[idx]=heap.push(make_pair(num,idx));
      printf("heap: create new iterator %p\n",it[idx]);
    } else {
      printf("heap:modify idx:%d to num:%d\n",idx,num);
      heap.modify(it[idx], make_pair(num, idx));
    }
    pair<int,int> hmin=heap.top();
    int mn = inf, midx = -1;
    for (int i = 1; i <= INDEX; ++i) {
      if (chk[i] < mn) {
        mn = chk[i];
        midx=i;
      }
    }
    //heap.print();
    printf("heapmin: min value:%d min index:%d, jury min value:%d min index:%d\n",hmin.first,hmin.second,mn,midx);
    if (mn != hmin.first || midx != hmin.second) {
      printf("bug\n");
      assert(0);
      return true;
    }
  }
  return false;
}
int main() {
  //single_case();
  //return 0;
  for (int t = 0; t < CASE; ++t) {
    printf("Case :%d\n",t);
    bool r = check_heap();
    if(r==1)break;
  }
  printf("all ok\n");
  return 0;
}