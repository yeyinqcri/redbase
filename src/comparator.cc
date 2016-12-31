#include "comparator.h"

#include <math.h>
#include <string.h>

namespace {
  bool AlmostEqualRelative(float a,
      float b,
      float maxRelativeError=0.000001) {
    if (a == b)
      return true;
    float relativeError = fabs((a - b) / b);
    if (relativeError <= maxRelativeError)
      return true;
    return false;
  }
}  // namspace

Comparator::Comparator(void* lValue, void* rValue, int attrLength, CompOp op) {
  this->lvalue_ = lValue;
  this->rvalue_ = rValue;
  this->attr_length_ = attrLength;
  this->op_ = op;
}

FloatComparator::FloatComparator(
    void* lValue, void* rValue, int attrLength, CompOp op) :
  Comparator(lValue, rValue, attrLength, op) {
}

bool FloatComparator::eval() {
  float l_value_float = *((float *)lvalue_);
  float r_value_float = *((float *)rvalue_);
  switch (op_) {
    case LE_OP:
      return l_value_float < r_value_float ||
        AlmostEqualRelative(l_value_float, r_value_float);
      break;
    case LT_OP:
      return l_value_float < r_value_float;
      break;
    case GE_OP:
      return l_value_float > r_value_float ||
        AlmostEqualRelative(l_value_float, r_value_float);
      break;
    case GT_OP:
      return l_value_float > r_value_float;
      break;
    case EQ_OP:
      return AlmostEqualRelative(l_value_float, r_value_float);
      break;
    case NE_OP:
      return !AlmostEqualRelative(l_value_float, r_value_float);
      break;
    case NO_OP:
      return true;
  }
  return true;
}

IntComparator::IntComparator(
    void* lValue, void* rValue, int attrLength, CompOp op) :
  Comparator(lValue, rValue, attrLength, op) {
}

bool IntComparator::eval() {
  int l_value_int = *((int *)lvalue_);
  int r_value_int = *((int *)rvalue_);
  switch (op_) {
    case LE_OP:
      return l_value_int <= r_value_int;
      break;
    case LT_OP:
      return l_value_int < r_value_int;
      break;
    case GE_OP:
      return l_value_int >= r_value_int;
      break;
    case GT_OP:
      return l_value_int > r_value_int;
      break;
    case EQ_OP:
      return l_value_int == r_value_int;
      break;
    case NE_OP:
      return l_value_int != r_value_int;
      break;
    case NO_OP:
      return true;
  }
  return true;
}

StringComparator::StringComparator(
    void* lValue, void* rValue, int attrLength, CompOp op) :
  Comparator(lValue, rValue, attrLength, op) {
}

bool StringComparator::eval() {
  char* lvalue_char = (char*) lvalue_;
  char* rvalue_char = (char*) rvalue_;
  switch (op_) {
    case LE_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) <= 0;
      break;
    case LT_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) < 0;
      break;
    case GE_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) >= 0;
      break;
    case GT_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) > 0;
      break;
    case EQ_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) == 0;
      break;
    case NE_OP:
      return strncmp(lvalue_char, rvalue_char, attr_length_) != 0;
      break;
    case NO_OP:
      return true;
  }
  return true;
}
