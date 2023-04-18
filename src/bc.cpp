//
// Created by triccsr on 4/1/23.
//
#include <bits/stdc++.h>

#include <algorithm>
#include <csetjmp>
#include <cstring>
#include <utility>

#include "config.hpp"
#include "fibheap.hpp"
#include "graph.hpp"

d_t *dis;
bool *ok;
FibHeap<std::pair<d_t, v_t>>::iterator *pqIter;
long double *sigma;
long double *deltaSrc;

vector<v_t> dijkstra(const KG<v_t, d_t> &kg, v_t src) {
  memset(ok, 0, sizeof(bool) * kg.size());
  memset(pqIter, 0, sizeof(FibHeap<std::pair<d_t, v_t>>::iterator) * kg.size());
  for(v_t i=0;i<kg.size();++i)sigma[i]=0.0;
  FibHeap<std::pair<d_t, v_t>> pq;
  dis[src] = DisType(0, 0);
  pqIter[src] = pq.push(std::make_pair(dis[src], src));
  sigma[src]=1;
  vector<v_t> ret;
  ret.reserve(kg.size());
  for (v_t _ = 0; _ < kg.size(); ++_) {
    if (pq.empty()) break;
    auto minPair = pq.top();
    pq.pop();
    ok[minPair.second] = true;
    ret.push_back(minPair.second);
    for (auto edge : kg.adjacent_edges(minPair.second)) {
      if (pqIter[edge.to] == nullptr) {
        dis[edge.to] = dis[minPair.second] + edge.weight;
        pqIter[edge.to] = pq.push(std::make_pair(dis[edge.to], edge.to));
        sigma[edge.to]=sigma[minPair.second];
      } else if (dis[minPair.second] + edge.weight < dis[edge.to]) {
        dis[edge.to] = dis[minPair.second] + edge.weight;
        pq.modify(pqIter[edge.to], std::make_pair(dis[edge.to], edge.to));
        sigma[edge.to]=sigma[minPair.second];
      } else if (dis[minPair.second] + edge.weight == dis[edge.to]) {
        sigma[edge.to]+=sigma[minPair.second];
      }
    }
  }
  return ret;
}

void update_bc(const KG<v_t, d_t> &kg, v_t src,long double bc[]) {
  vector<v_t> dagSort = dijkstra(kg, src);
  if (dagSort.empty()) {
    std::cerr << "warning: dagSort is empty" << std::endl;
    return;
  }
  //for(v_t i=0;i<kg.size();++i)deltaSrc[i]=0.0;
  for (v_t i = dagSort.size() - 1; i >= 0; --i) {
    v_t v = dagSort[i];
    deltaSrc[v]=0.0;
    for (auto edge : kg.adjacent_edges(v)) {
      v_t w = edge.to;
      deltaSrc[v]+=sigma[v]/sigma[w]*(1+sigma[w]);
    } 
  }
  for (v_t i = 0; i < kg.size(); ++i) {
    bc[i]+=deltaSrc[i];
  }
}

void get_bc(const KG<v_t, d_t> &kg, int pivotNum,long double bc[]) {
  dis = new d_t[kg.size()];
  ok = new bool[kg.size()];
  pqIter = new FibHeap<std::pair<d_t, v_t>>::iterator[kg.size()];
  sigma = new long double[kg.size()];
  deltaSrc = new long double[kg.size()];

  auto *degDesc = new std::pair<v_t, v_t>[kg.size()];
  for (v_t v = 0; v < kg.size(); ++v) {
    bc[v]=0.0;
    degDesc[v].first = kg.degree(v);
    degDesc[v].second = v;
  }

  std::sort(degDesc, degDesc + kg.size(), std::greater<>());

  for (int i = 0; i < pivotNum; ++i) {
    v_t src = degDesc[i].second;
    update_bc(kg,src,bc);
  }

  delete[] dis;
  delete[] ok;
  delete[] pqIter;
  delete[] sigma;
  delete [] deltaSrc;
}