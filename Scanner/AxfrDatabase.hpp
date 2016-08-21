//
// Created by marcin on 01.08.16.
//

#ifndef MYDIG_AXFRDATABASE_HPP
#define MYDIG_AXFRDATABASE_HPP

#include <dig_parser.h>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "ScanningResult.hpp"

class AxfrDatabase {
public:
    AxfrDatabase()
    {}

    ~AxfrDatabase()
    {
        for(std::vector<ScanningResult*>::iterator it = data.begin() ; it != data.end(); ++it)
            delete *it;
    }

    void addToLocalDb(response_t* res)
    {
        if( res->cls == NULL || res->type == NULL || res == NULL)
        {
            return;
        }

	bool cl = std::string("SOA").compare(res->cls);
	bool ty = std::string("SOA").compare(res->type);
	if(cl && ty) {
	  std::cout<<res->name<<std::endl;
	  ScanningResult* temp = new ScanningResult(res);
	  if(temp != NULL)
	    data.push_back(temp);
	}
	else
	  return;
    }

    void addRdata()
    {
        if(this->data.empty())
            return;
	    boost::property_tree::ptree tree;
        for(auto i : this->data) {
	        boost::property_tree::ptree& book = tree.add("axfrlookup.domain", "");
	        book.add("type", i->get_type());
	        book.add("www_address", i->get_name());
	        book.add("rdata", i->get_rdata());
        }
//        if(!this->data.empty()) {
//            std::cout<<this->data.back()->get_name() + ".xml"<<std::endl;
//	        boost::property_tree::write_xml(std::string(this->data.back()->get_name()) + ".xml", tree,
//					  std::locale(),
//					  boost::property_tree::xml_writer_make_settings<std::string>(' ', 1));
//	    }
    }

    AxfrDatabase(const AxfrDatabase&) = delete;
    AxfrDatabase operator=(const AxfrDatabase &a) = delete;
    std::vector<ScanningResult*>* get_result()
    {
      return &data;
    }
private:
    std::vector<ScanningResult*> data;
};


#endif //MYDIG_AXFRDATABASE_HPP
