//
// Created by marcin on 27.07.16.
//

#include "Scanner/axfrLookup.h"
#include "Scanner/AxfrDatabase.hpp"
#include "Scanner/InputReader.h"

extern "C"
{
  #include "digUtils/dig_parser.h"
  axfrLookup* axfrLookup_new()
  {
    return new axfrLookup();
  }
  
  void axfrLookup_performLookup(axfrLookup* lookup, const char* domain, const char* ns)
  {
    lookup->performLookup(domain, ns);
  }

  std::vector<ScanningResult*>* axfrLookup_getResult(axfrLookup* lookup)
  {
    return lookup->get_domains();
  }

  std::vector<std::string> axfrLookup_getReturnedDomains(axfrLookup* lookup)
  {
    std::vector<std::string> data;
    
    for(int i = 0; i<lookup->get_domains()->size(); ++i)
    {
      data.push_back(lookup->get_domains()->at(i)->get_rdata());
    }

    return data;
  }

  std::vector<std::string> axfrLookup_getDumbVector(axfrLookup* lookup)
  {
    std::vector<std::string> to_return = {"dns1.onet.pl", "dns2.onet.pl", "dns3.onet.pl"};
    return to_return;
  }
  std::vector<int> axfrLookup_getDumbVectorInt()
  {
    std::vector<int> to_return = {1, 2, 3};
    return to_return;
  }

  int axfrLookup_getDumbSize(std::vector<std::string> vec)
  {
    return vec.size();
  }

  int axfrLookup_getSizeOfReturnedData(axfrLookup* lookup)
  {
    return lookup->get_domains()->size();
  }

  void print_data(std::vector<ScanningResult*>* data)
  {
    if(data == NULL)
      return;
    std::cout<<data->size()<<std::endl;
    for(int i = 0; i<data->size(); ++i)
      std::cout<<"costam "<<data->size()<<" "<<i<<std::endl;
  }

  std::string* get_names(std::vector<ScanningResult*>* data)
  {
    if(data == NULL)
      return NULL;
    std::string* tmp = new std::string[data->size()];
    for(int i = 0; i<data->size(); ++i)
      tmp[i]=data->at(i)->get_name();

    return tmp;
  }
  
  void axfrLookup_destroy(axfrLookup* lookup)
  {
    delete lookup;
  }
}
