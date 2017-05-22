#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CPPace.Lib/ConsList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CPPaceTests {
  TEST_CLASS(ConsListTests) {
  public:

    TEST_METHOD(Cons) {
      shared_ptr<ConsList<int>> list = ConsList<int>::create(1);
      
      list = ConsList<int>::cons(2, list);
      
      Assert::AreEqual(list->value, 2);
      Assert::AreEqual(list->prev->value, 1);
      
      shared_ptr<ConsList<int>> list1 = ConsList<int>::cons(14, ConsList<int>::cons(13, list));
      shared_ptr<ConsList<int>> list2 = ConsList<int>::cons(24, ConsList<int>::cons(23, list));

      Assert::AreEqual(list1->value, 14);
      Assert::AreEqual(list1->prev->value, 13);
      Assert::AreEqual(list1->prev->prev->value, 2);
      Assert::AreEqual(list1->prev->prev->prev->value, 1);


      Assert::AreEqual(list2->value, 24);
      Assert::AreEqual(list2->prev->value, 23);
      Assert::AreEqual(list2->prev->prev->value, 2);
      Assert::AreEqual(list2->prev->prev->prev->value, 1);

    }

    TEST_METHOD(ToVector) {
      shared_ptr<ConsList<int>> list = ConsList<int>::create(1);
      list = ConsList<int>::cons(2, list);
      list = ConsList<int>::cons(3, list);
      list = ConsList<int>::cons(4, list);
      list = ConsList<int>::cons(5, list);
      list = ConsList<int>::cons(6, list);
      list = ConsList<int>::cons(7, list);
      list = ConsList<int>::cons(8, list);

      vector<int> vector = ConsList<int>::to_vector(list);
      Assert::AreEqual(8, vector.front());
      Assert::AreEqual(7, vector[1]);
      Assert::AreEqual(6, vector[2]);
      Assert::AreEqual(5, vector[3]);
      Assert::AreEqual(4, vector[4]);
      Assert::AreEqual(3, vector[5]);
      Assert::AreEqual(2, vector[6]);
      Assert::AreEqual(1, vector[7]);
      Assert::AreEqual(8, (int)vector.size());
    }

  };
}