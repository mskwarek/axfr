//
// Created by marcin on 27.07.16.
//

#include "Scanner/axfrLookup.h"
#include "Scanner/AxfrDatabase.hpp"
#include "Scanner/InputReader.h"

int main()
{
  axfrLookup* lookup = new axfrLookup();
   
  for(int i=0; i<1; ++i){
    lookup->performLookup("infusionsoft.com", "ns3.infusionsoft.com");
    
  }
   delete lookup;
    return 0;
}

