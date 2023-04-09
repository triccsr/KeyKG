//
// Created by triccsr on 4/1/23.
//
#include "config.hpp"
#include "fibheap.hpp"
#include "graph.hpp"
#include <algorithm>
#include <bits/stdc++.h>

void get_bc(const KG<v_t, e_t> &kg, int pivotNum) {
  auto *degDesc = new std::pair<v_t, v_t>[kg.size()];
  for (v_t v = 0; v < kg.size(); ++v) {
    degDesc[v].first = kg.degree(v);
    degDesc[v].second = v;
  }
  std::sort(degDesc, degDesc + kg.size(), std::greater<>());
  for (int i = 0; i < pivotNum; ++i) {
    v_t ver = degDesc[i].second;
  }
}