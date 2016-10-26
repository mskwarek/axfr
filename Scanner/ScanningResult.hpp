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
        std::string tmp = std::to_string(response->ttl);
        this->ttl = (char*) tmp.c_str();
    }

    ScanningResult(const std::string _name, const std::string _ip, 
        const std::string _ttl, const std::string _cls, 
        const std::string _type, const std::string _rdata)
  {
    name = (char*) malloc(sizeof(char)*(_name.length() + 1));
    ip = (char*) malloc(sizeof(char)*(_ip.length() + 1));
    ttl = (char*) malloc(sizeof(char)*(_ttl.length() + 1));
    cls = (char*) malloc(sizeof(char)*(_cls.length() + 1));
    type = (char*) malloc(sizeof(char)*(_type.length() + 1));
    rdata = (char*) malloc(sizeof(char)*(_rdata.length() + 1));

            snprintf(name, _name.length() + 1, "%s", _name.c_str());
            snprintf(ip, _ip.length() + 1, "%s", _ip.c_str());
            snprintf(ttl, _ttl.length() + 1, "%s", _ttl.c_str());
            snprintf(cls, _cls.length() + 1, "%s", _cls.c_str());
            snprintf(type, _type.length() + 1, "%s", _type.c_str());
            snprintf(rdata, _rdata.length() + 1, "%s", _rdata.c_str());
    }

    ~ScanningResult(){
        // std::cout<<"d_ctor c++ "<<ip<<std::endl;
      if(name!=NULL)
	{
	  free(name);
	}
      if(ip!=NULL)
	{
	  free(ip);
	}
      if(ttl!=NULL)
	{
	  free(ttl);
	}
      if(cls!=NULL)
	{
	  free(cls);
	}
      if(type=NULL)
	{
	  free(type);
	}
      if(rdata!=NULL)
	{
	  free(rdata);
	}
    }

    char* get_name()
    {
        return this->name;
    }
    char* get_ip()
    {
        return this->ip;
    }

    char* get_cls()
    {
        return this->cls;
    }

    char* get_type()
    {
        return this->type;
    }

    char* get_rdata()
    {
        return this->rdata;
    }

    char* get_ttl()
    {
        return this->ttl;
    }

private:
    char* name;
    char* ip;
    char* ttl;
    char* cls;
    char* type;
    char* rdata;
};
