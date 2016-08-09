#include <string>


class ScannedDomain{
public:
    ScannedDomain():
            domain_address(),
            ns_address()
    {}

    ~ScannedDomain() {}

    ScannedDomain(std::string domain, std::string ns):
            domain_address(domain),
            ns_address(ns)
    {}

    std::string GetDomainAddress()
    {
        return domain_address;
    }
    std::string GetNsAddress()
    {
        return ns_address;
    }

private:
    std::string domain_address;
    std::string ns_address;
};