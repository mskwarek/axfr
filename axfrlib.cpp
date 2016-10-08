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

  const char* axfrLookup_getReturnedDomain(axfrLookup* lookup, int domain_index)
  {
    return lookup->get_domains()->at(domain_index)->get_rdata().c_str();
  }

  int axfrLookup_getSizeOfReturnedData(axfrLookup* lookup)
  {
    try
      {
	return lookup->get_domains()->size();
      }
    catch(...)
      {
	return 0;
      }
  }

  int axfrLookup_getSize(std::vector<std::string>* vec)
  {
    return vec->size();
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




  std::vector<std::string> to_return = {"dns1.onet.pl", "dns2.onet.pl", "dns3.onet.pl"};
   
  std::vector<std::string>* axfrLookup_getDumbVector()
  {
    return new std::vector<std::string>();
  }

  void axfrLookup_fillWithData(std::vector<std::string>* vec)
  {
    vec->push_back("dns1.onet.pl");
    vec->push_back("dns2.onet.pl");
    vec->push_back("dns3.onet.pl");
  }
  
  int axfrLookup_getDumbSize(std::vector<std::string>* vec)
  {
    return vec->size();
  }

  const char* axfrLookup_get_to_return(std::vector<std::string>* vec, int index)
  {
    std::cout<<index<<std::endl;
    if(index < vec->size() && index >= 0)
      {
	return vec->at(index).c_str();
      }
    else
      {
	return "";
      }
  }
}
