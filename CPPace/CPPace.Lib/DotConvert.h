#pragma once

#ifdef DOTCONVERT_EXPORTS
#define DOTCONVERT_API _declspec(dllexport)
#else
#define DOTCONVERT_API _declspec(dllimport)
#endif

#include <iostream>
#include "AdjacencyList.h"

using namespace std;

class DOTCONVERT_API DotConvert {
public:
  static void write_to_stream(AdjacencyList graph, ostream& stream);
};
