//
// Created by triccsr on 4/1/23.
//

#ifndef DEF_H
#define DEF_H 0
#include <cstddef>
#include <cstdint>
typedef uint64_t  v_t;

/*struct DisType{
    double len;
    uint64_t edgeNum;
    DisType() = default;
    DisType(double len, uint64_t edgeNum) : len(len), edgeNum(edgeNum) {}
    bool operator<(const DisType &d) const {
      return (len == d.len) ? (edgeNum < d.edgeNum) : (len < d.len);
    }
    bool operator==(const DisType &d) const {
      return (len==d.len)&&(edgeNum==d.edgeNum);
    }
};
DisType operator+(const DisType &a, const DisType &b) {
    return {a.len+b.len,a.edgeNum+b.edgeNum};
}

typedef DisType d_t;*/

typedef double d_t;
typedef uint64_t k_t;

//const uint64_t llinf = 1e18;
const double D_INF=1e18;
const double eps=1e-6;
const int G=11;

#endif //DEF_H
