#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib.h"

int main(int, char const **)
{
    try
    {
        IpAddrPool ip_pool;
        std::string line;
        while (std::getline(std::cin, line))
        {
            auto v = split(line, '\t');
            ip_pool.insert(v[0]);
        }

        ip_pool.print_all(std::cout);

        ip_pool.print_all(std::cout, 1);

        ip_pool.print_all(std::cout, 46, 70);

        ip_pool.print_any(std::cout, 46);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
