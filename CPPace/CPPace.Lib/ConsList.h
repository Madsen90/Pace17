#pragma once

#include <memory>
#include <vector>

using namespace std;


template <typename T>
class ConsList {
public:
  T value;
  shared_ptr<ConsList<T>> prev;

  static shared_ptr<ConsList<T>> cons(T value, shared_ptr<ConsList<T>>& tail) {
    return make_shared<ConsList<T>>(value, tail);
  }

  static shared_ptr<ConsList<T>> cons(T value, shared_ptr<ConsList<T>>&& tail) {
    return make_shared<ConsList<T>>(value, tail);
  }

  static shared_ptr<ConsList<T>> create(T value) {
    return make_shared<ConsList<T>>(value);
  }
  
  static vector<T> to_vector(shared_ptr<ConsList<T>>& list) {
    return to_vector((shared_ptr<ConsList<T>>&&)list);
  }

  static vector<T> to_vector(shared_ptr<ConsList<T>>&& list) {
    vector<T> result;
    shared_ptr<ConsList<T>> next = list;
    do {
      result.emplace_back(next->value);
    } while ((next = next->prev));
    return result;
  }


  ConsList(T value)
    : value(value),
    prev()
  { }

  ConsList(T value, shared_ptr<ConsList<T>>& tail)
    : value(value),
    prev(tail)
  { }

};
