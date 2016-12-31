// This library is used to define the comparators and their logic of how to eval
// the comparison respectively.

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "rm.h"

class Comparator {
 public:
   Comparator(void* lValue, void* rValue, int attrLength, CompOp op);
   virtual bool eval() = 0;
 protected:
   void* lvalue_;
   void* rvalue_;
   int attr_length_;
   CompOp op_;
};

class FloatComparator : public Comparator {
 public:
   FloatComparator(void* lValue, void* rValue, int attrLength, CompOp op);
   bool eval();
};

class IntComparator : public Comparator {
 public:
   IntComparator(void* lValue, void* rValue, int attrLength, CompOp op);
   bool eval();
};

class StringComparator : public Comparator {
 public:
   StringComparator(void* lValue, void* rValue, int attrLength, CompOp op);
   bool eval();
};

#endif
