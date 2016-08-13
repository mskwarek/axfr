#include <iostream>
#include <string>

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
            ttl(response->ttl),
            cls(response->cls),
            type(response->type),
            rdata(response->rdata)
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

    int32_t get_ttl()
    {
        return this->ttl;
    }

private:
    std::string name;
    std::string ip;
    int32_t ttl;
    std::string cls;
    std::string type;
    std::string rdata;
};
