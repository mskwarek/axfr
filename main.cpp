//
// Created by marcin on 27.07.16.
//

#include "Scanner/axfrLookup.h"
#include "Scanner/AxfrDatabase.hpp"
#include "Scanner/InputReader.h"

int main(int argc, char* argv[])
{
  axfrLookup* lookup = new axfrLookup();
   
  for(int i=0; i<1; ++i){
    lookup->performLookup("onet.pl", "@dns1.onet.pl");
    
  }

    lookup->print();

   delete lookup;

    return 0;
}

