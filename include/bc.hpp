#ifndef BC_H
#define BC_H 0
#include "def.hpp"
#include "graph.hpp"
#include <vector>
namespace NAMESPACE_BC {
  std::vector<long double> get_bc(const KG<v_t, d_t> &kg, uint32_t pivotNum);
}
#endif