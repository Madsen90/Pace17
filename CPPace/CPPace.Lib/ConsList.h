#pragma once
#include "EXPORT.h"

#include <memory>
#include <vector>

using namespace std;

template <typename T>
class DLLEXPORT ConsList {
public:
  T value;
  shared_ptr<ConsList<T>> prev;
  
  ConsList(T value)
    : value(value),
      prev()
  { }

  ConsList(T value, shared_ptr<ConsList<T>> tail)
    : value(value),
      prev(tail)
  { }

  ConsList<T> cons(T value) {
    return ConsList<T>(value, shared_ptr<ConsList<T>>(this));
  }
  
  vector<T> to_vector() {
    vector<T> result;
    shared_ptr<ConsList<T>> next(this);
    do {
      result.emplace_back(next->value);
    } while ((next = next->prev));
    return result;
  }
};
