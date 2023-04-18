//
// Created by triccsr on 4/1/23.
//

#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H
#include <vector>
template <typename V, typename E> struct Edge {
  V to;
  E weight;
  Edge()= default;
  Edge(V to,E weight):to(to),weight(weight){}
};

template <typename V, typename E> class KG {
  std::vector<std::vector<Edge<V, E>>> e;
public:
  KG() {
    for (auto v : e) {
      v.clear();
    }
    e.clear();
  }
  void add_edge(V u, V v, E l);
  const std::vector<Edge<V, E>> &adjacent_edges(V vertexIndex) const;
  V degree(V vertexIndex) const;
  V size() const;
};

using std::max;

template <typename V, typename E> void KG<V, E>::add_edge(V u, V v, E l) {
  if (max(u, v) >= e.size()) {
    e.resize(max(u, v) + 1, std::vector<Edge<V, E>>());
  }
  e[u].push_back(Edge<V, E>{v, l});
  e[v].push_back(Edge<V, E>{u, l});
}
template <typename V, typename E>
const std::vector<Edge<V, E>> &KG<V, E>::adjacent_edges(V vertexIndex) const{
  return e[vertexIndex];
}
template <typename V, typename E> V KG<V, E>::degree(V vertexIndex) const {
  return e[vertexIndex].size();
}
template <typename V, typename E> V KG<V, E>::size() const {
  return KG::e.size();
}
#endif // CODE_GRAPH_H
