#include <iostream>
#include <algorithm>

#include "version.h"
#include "lib.h"

bool Compare(int value, int data)
{
    return value == data;
}

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void IpAddrPool::insert(const IpAddr& addr)
{
    ++container[addr];
}

void IpAddrPool::insert(const std::string& str)
{
    auto v = split(str, '.');
    auto tuple = MakeTupleFromVector<IpAddr>::Make(v);
    insert(tuple);
}

void IpAddrPool::print_filtered(std::ostream& ostream, std::function<bool(IpAddr)> filter)
{
    for(const auto& ip_quantity : container)
    {
        if(!filter(ip_quantity.first))
            continue;

        for(int i = 0; i < ip_quantity.second; i++)
        {
            PrintTuple(ostream, ip_quantity.first);
            ostream << std::endl;
        }
    }
}

int version()
{
    return PROJECT_VERSION_PATCH;
}
