#include "SetFunctions.h"

set<int> SetFunctions::set_union_two(set<int>& s1, set<int>& s2) {
  set<int> set_union;
  auto s1_it = s1.begin();
  auto s2_it = s2.begin();
  while (true) {
    //If either set is empty, copy the rest of the other
    if (s1_it == s1.end()) {
      while (s2_it != s2.end()) {
        set_union.emplace(*s2_it);
        s2_it++;
      }
      return set_union;
    }
    else if (s2_it == s2.end()) {
      while (s1_it != s1.end()) {
        set_union.emplace(*s1_it);
        s1_it++;
      }
      return set_union;
    }

    //Both elements are identical, take one, increment both
    if (*s1_it == *s2_it) { 
      set_union.emplace(*s1_it);
      s1_it++;
      s2_it++;
    }
    //First element is lowest, take it and increment outer
    else if (*s1_it < *s2_it) { 
      set_union.emplace(*s1_it);
      s1_it++;
    }
    //Second element must be lowest, take it and increment inner
    else { 
      set_union.emplace(*s2_it);
      s2_it++;
    }
  }
  return set_union;
}

set<int> SetFunctions::set_intersect_two(set<int>& s1, set<int>& s2) {
  set<int> set_intersect;
  auto s1_it = s1.begin();
  auto s2_it = s2.begin();
  while (s1_it != s1.end() 
    && s2_it != s2.end()) {
    //Both elements are the same. Add and increment both.
    if (*s1_it == *s2_it) {
      set_intersect.emplace(*s1_it);
      s1_it++;
      s2_it++;
    }
    //First element is lowest. Increment first.
    else if (*s1_it < *s2_it) {
      s1_it++;
    }
    //Second element is lowest. Increment second.
    else {
      s2_it++;
    }
  }

  return set_intersect;
}

set<int> SetFunctions::set_intersect_three(set<int>& s1, set<int>& s2, set<int>& s3) {
  set<int> set_intersect;
  auto s1_it = s1.begin();
  auto s2_it = s2.begin();
  auto s3_it = s3.begin();
  while (s1_it != s1.end() //While none are empty
    && s2_it != s2.end() 
    && s3_it != s3.end()) {
    //All elements are the same. Add and increment all.
    if (*s1_it == *s2_it
      && *s1_it == *s3_it) {
      set_intersect.emplace(*s1_it);
      s1_it++;
      s2_it++;
      s3_it++;
    }
    //An element is higher than the others. Increment others
    else {      
      if (*s1_it < *s3_it)
        s1_it++;
      else if (*s3_it < *s1_it)
        s3_it++;
      if (*s2_it < *s3_it)
        s2_it++;
      else if (*s3_it < *s2_it)
        s3_it++;      
    }
  }

  return set_intersect;
}