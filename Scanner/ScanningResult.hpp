#include <iostream>
#include <string>
#include <stdlib.h>

class ScanningResult
{
public:
    ScanningResult():
            name(),
            ip(),
            ttl(),
            cls(),
            type(),
            rdata()
    {}
    ScanningResult(response_t* response):
            name(response->name),
            ip(response->ip),        
            cls(response->cls),
            type(response->type),
            rdata(response->rdata)
    {
        this->ttl = std::to_string(response->ttl);
    }

    ScanningResult(const std::string _name, const std::string _ip, 
        const std::string _ttl, const std::string _cls, 
        const std::string _type, const std::string _rdata):
            name(_name),
            ip(_ip),
            ttl(_ttl),
            cls(_cls),
            type(_type),
            rdata(_rdata)
    {}

    ~ScanningResult(){
        std::cout<<rdata<<std::endl;
    }

    std::string get_name()
    {
        return this->name;
    }
    std::string get_ip()
    {
        return this->ip;
    }

    std::string get_cls()
    {
        return this->cls;
    }

    std::string get_type()
    {
        return this->type;
    }

    std::string get_rdata()
    {
        return this->rdata;
    }

    std::string get_ttl()
    {
        return this->ttl;
    }

private:
    std::string name;
    std::string ip;
    std::string ttl;
    std::string cls;
    std::string type;
    std::string rdata;
};
