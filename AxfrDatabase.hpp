//
// Created by marcin on 01.08.16.
//

#ifndef MYDIG_AXFRDATABASE_HPP
#define MYDIG_AXFRDATABASE_HPP

#include <dig_parser.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

class AxfrDatabase {
public:
    static AxfrDatabase& getInstance()
    {
        static AxfrDatabase instance;
        return instance;
    }

    void addToLocalDb(response_t res, std::vector<std::string> tokens)
    {
//        std::cout<<"pisze\n";
        for (auto const &n : tokens) {
            bool cl = std::string("SOA").compare(res.cls);
            bool ty = std::string("SOA").compare(res.type);
            if(cl && ty) {
                response_t temp = res;
                //memcpy(&temp, res, sizeof(response_t));
                data.push_back(temp);
            }
            else
                break;
        }
    }

    AxfrDatabase addRdata()
    {
        boost::property_tree::ptree tree;
        for(auto i : this->data) {
            boost::property_tree::ptree& book = tree.add("axfrlookup.domain", "");
            book.add("type", i.type);
            book.add("www_address", i.name);
            book.add("rdata", i.rdata);
        }
        if(!this->data.empty()) {
            write_xml(std::string(this->data.back().name) + ".xml", tree,
                    std::locale(),
                    boost::property_tree::xml_writer_settings<char>(' ', 4));
        }
    }

    AxfrDatabase(const AxfrDatabase&) = delete;
    AxfrDatabase operator=(const AxfrDatabase &a) = delete;
private:
    AxfrDatabase(){};
    std::vector<response_t> data;
};


#endif //MYDIG_AXFRDATABASE_HPP
