#include "def.hpp"
#include <cmath>
bool double_le(double a,double b){
  return a+eps<b;  
}
bool double_ge(double a,double b){
  return b+eps<a;
}
bool double_eq(double a,double b){
  return fabs(a-b)<eps;
}