//
// Created by triccsr on 4/1/23.
//

#ifndef CODE_CONFIG_H
#define CODE_CONFIG_H
#include <cstddef>
#include <cstdint>
typedef unsigned long long  v_t;

struct DisType{
    uint64_t len;
    uint64_t edgeNum;
    DisType() = default;
    DisType(uint64_t len, uint64_t edgeNum) : len(len), edgeNum(edgeNum) {}
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
typedef DisType d_t;

const uint64_t llinf=1e18;


#endif //CODE_CONFIG_H
