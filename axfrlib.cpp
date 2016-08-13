//
// Created by marcin on 27.07.16.
//

#include "Scanner/axfrLookup.h"
#include "Scanner/AxfrDatabase.hpp"
#include "Scanner/InputReader.h"

extern "C"
{
  axfrLookup* axfrLookup_new()
  {
    return new axfrLookup();
  }
  
  void axfrLookup_performLookup(axfrLookup* lookup, const char* domain, const char* ns)
  {
    lookup->performLookup(domain, ns);
  }
}
