//
// Created by triccsr on 4/1/23.
//
#include <bits/stdc++.h>

#include <algorithm>
#include <cmath>
#include <csetjmp>
#include <cstddef>
#include <cstring>
#include <utility>
#include <vector>

#include "def.hpp"
#include "fibheap.hpp"
#include "graph.hpp"
#include "double.hpp"

namespace NAMESPACE_BC {
  vector<d_t> dis;
  vector<bool> ok;
  vector<FibHeap<std::pair<d_t,v_t>>::iterator> pqIter;
  vector<uint64_t> sigma;
  vector<long double> deltaSrc;

  vector<v_t> dijkstra(const KG<v_t, d_t> &kg, v_t src) {
    // vector<bool> ok(kg.size(),false);
    // vector<FibHeap<std::pair<d_t,v_t> >::iterator> pqIter(kg.size(),nullptr);
    // memset(ok, 0, sizeof(bool) * kg.size());
    // memset(pqIter, 0, sizeof(FibHeap<std::pair<d_t, v_t>>::iterator) * kg.size());
    for (v_t i = 0; i < kg.size(); ++i){
      ok[i]=false;
      pqIter[i]=nullptr;
      sigma[i] = 0;
    }
    FibHeap<std::pair<d_t, v_t>> pq;
    dis[src] = 0.0;
    pqIter[src] = pq.push(std::make_pair(dis[src], src));
    sigma[src] = 1;
    vector<v_t> ret;
    ret.reserve(kg.size());
    for (v_t _ = 0; _ < kg.size(); ++_) {
      if (pq.empty()) break;
      auto minPair = pq.top();
      pq.pop();
      ok[minPair.second] = true;
      ret.push_back(minPair.second);
      for (auto edge : kg.adjacent_edges(minPair.second)) {
        if (ok[edge.to] == true) continue;
        d_t newDist=dis[minPair.second]+edge.weight;
        if (pqIter[edge.to] == nullptr) {
          dis[edge.to] = newDist;
          pqIter[edge.to] = pq.push(std::make_pair(dis[edge.to], edge.to));
        } else if (double_le(newDist, dis[edge.to])) {
          dis[edge.to] = newDist;
          pq.modify(pqIter[edge.to], std::make_pair(dis[edge.to], edge.to));
        }       
        if(double_eq(dis[edge.to],newDist)){
          sigma[edge.to]+=sigma[minPair.second];
        }
      }
    }
    return ret;
  }

  void update_bc(const KG<v_t, d_t> &kg, v_t src, vector<long double> &bc) {
    vector<v_t> dagSort = dijkstra(kg, src);
    if (dagSort.empty()) {
      std::cerr << "warning: dagSort is empty" << std::endl;
      return;
    }
    
    for(v_t i=0;i<kg.size();++i)deltaSrc[i]=0.0;
    for (auto rit=dagSort.rbegin();rit!=dagSort.rend();++rit) {
      v_t w=*rit;
      for (auto edge : kg.adjacent_edges(w)) {
        v_t v = edge.to;
        if (double_eq(dis[v] + edge.weight, dis[w])) {  // v is a precedor of w on sssp DAG
          deltaSrc[v] += (long double)sigma[v] / (long double)sigma[w] * (1.0 + deltaSrc[w]);
        }
      }
      if(w!=src){
        bc[w]+=deltaSrc[w];
      }
    }
  }
  

  vector<long double> get_bc(const KG<v_t, d_t> &kg, uint32_t pivotNum) {
    vector<long double> bc;
    bc.resize(kg.size());

    dis.resize(kg.size());
    ok.resize(kg.size());
    pqIter.resize(kg.size());
    sigma.resize(kg.size());
    deltaSrc.resize(kg.size());
    
    vector<std::pair<v_t,v_t>> degDesc(kg.size());
    for (v_t v = 0; v < kg.size(); ++v) {
      bc[v] = 0.0;
      degDesc[v].first = kg.degree(v);
      degDesc[v].second = v;
    }

    std::sort(degDesc.begin(), degDesc.end(), std::greater<>());

    for (uint32_t i = 0; i < pivotNum; ++i) {
      v_t src = degDesc[i].second;
      update_bc(kg, src, bc);
    }
    return bc;
  }
}