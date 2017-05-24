#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/SetFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(SetFunctionsTests) {
public:

  TEST_METHOD(UniqueElementsCheck) {
    set<int> s1;

    s1.emplace(1);
    s1.emplace(1);
    s1.emplace(1);
    s1.emplace(1);

    Assert::IsTrue(1 == s1.size());
  }

  TEST_METHOD(SortedCheck) {
    set<int> s1;
    set<int> s2;
    s1.emplace(3);
    s1.emplace(2);
    s2.emplace(2);
    s2.emplace(3);

    Assert::IsTrue(SetFunctions::set_union_two(s1, s2).size() == SetFunctions::set_union_two(s1, s1).size()
      && SetFunctions::set_union_two(s1, s2).size() == SetFunctions::set_union_two(s2, s2).size());
  }

  TEST_METHOD(UnionTwo) {
    set<int> s1;
    set<int> s2;

    //{} and {}
    Assert::IsTrue(0 == SetFunctions::set_union_two(s1, s2).size());

    //{1} and {2}
    s1.emplace(1);
    s2.emplace(2);
    Assert::IsTrue(2 == SetFunctions::set_union_two(s1, s2).size());

    //{1, 2} and {2, 3}
    s1.emplace(2);
    s2.emplace(3);
    Assert::IsTrue(3 == SetFunctions::set_union_two(s1, s2).size());

    //{1, 2, 3} and {2, 3}
    s1.emplace(3);
    Assert::IsTrue(3 == SetFunctions::set_union_two(s1, s2).size());

    //{1, 2, 3} and {2, 3, 4, 5}
    s2.emplace(4);
    s2.emplace(5);
    Assert::IsTrue(5 == SetFunctions::set_union_two(s1, s2).size());
  }

  TEST_METHOD(IntersectTwo) {
    set<int> s1;
    set<int> s2;

    //{} and {}
    Assert::IsTrue(0 == SetFunctions::set_intersect_two(s1, s2).size());

    //{1} and {2}
    s1.emplace(1);
    s2.emplace(2);
    Assert::IsTrue(0 == SetFunctions::set_intersect_two(s1, s2).size());

    //{1, 2} and {2, 3}
    s1.emplace(2);
    s2.emplace(3);
    Assert::IsTrue(1 == SetFunctions::set_intersect_two(s1, s2).size());

    //{1, 2, 3} and {2, 3}
    s1.emplace(3);
    Assert::IsTrue(2 == SetFunctions::set_intersect_two(s1, s2).size());

    //{1, 2, 3, 4} and {2, 3, 4, 5}
    s1.emplace(4);
    s2.emplace(4);
    s2.emplace(5);
    Assert::IsTrue(3 == SetFunctions::set_intersect_two(s1, s2).size());
  }

  TEST_METHOD(IntersectThree) {
    set<int> s1;
    set<int> s2;
    set<int> s3;

    //{} and {} and {}
    Assert::IsTrue(0 == SetFunctions::set_intersect_three(s1, s2, s3).size());

    //{1} and {1} and {1}
    s1.emplace(1);
    s2.emplace(1);
    s3.emplace(1);
    Assert::IsTrue(1 == SetFunctions::set_intersect_three(s1, s2, s3).size());

    //{1, 2} and {1} and {1}
    s1.emplace(2);
    Assert::IsTrue(1 == SetFunctions::set_intersect_three(s1, s2, s3).size());

    //{1, 2} and {1, 3} and {1, 4}
    s2.emplace(3);
    s3.emplace(4);
    Assert::IsTrue(1 == SetFunctions::set_intersect_three(s1, s2, s3).size());

    //{1, 2, 3, 4, 5} and {1, 3} and {1, 4}
    s1.emplace(3);
    s1.emplace(4);
    s1.emplace(5);
    Assert::IsTrue(1 == SetFunctions::set_intersect_three(s1, s2, s3).size());

    //{1, 2, 3, 4, 5} and {1, 3, 4} and {1, 3, 4}
    s2.emplace(4);
    s3.emplace(3);
    Assert::IsTrue(3 == SetFunctions::set_intersect_three(s1, s2, s3).size());
  }
  };
}