#define BOOST_TEST_MODULE ip_filter_test

#include <boost/test/unit_test.hpp>

#include "lib.h"

BOOST_AUTO_TEST_SUITE(ip_filter_test)

struct Fixture
{
    Fixture()
    {
        static const char* input = R"(113.162.145.156	111	0
157.39.22.224	5	6
79.180.73.190	2	1
179.210.145.4	22	0
219.102.120.135	486	0
67.232.81.208	1	0
85.254.10.197	0	7
23.240.215.189	6	0
1.29.168.152	17	0
185.69.186.168	1	3
110.152.103.161	0	1
185.182.81.131	1	1
68.45.152.8	2	0
114.221.179.117	0	1
185.89.100.249	752	0
46.70.113.73	1	0
188.68.0.38	1	0
119.42.115.97	11	0
194.106.108.214	0	3
24.210.239.177	1	0
166.88.193.116	9	0
62.205.202.214	0	2
188.166.87.191	0	50
103.62.147.2	1	6
95.181.177.197	1	0
95.181.177.197	1	0
149.107.0.27	3	0
182.186.69.18	96	0
73.21.168.251	2	0)";

        auto lines = split(input, '\n');
        for (const auto& line : lines)
        {
            pool.insert(split(line, '\t')[0]);
        }
    }

    IpAddrPool pool;
};

BOOST_FIXTURE_TEST_CASE(test_reverse_lexicographycal_sort, Fixture)
{
    static const char* output = R"(219.102.120.135
194.106.108.214
188.166.87.191
188.68.0.38
185.182.81.131
185.89.100.249
185.69.186.168
182.186.69.18
179.210.145.4
166.88.193.116
157.39.22.224
149.107.0.27
119.42.115.97
114.221.179.117
113.162.145.156
110.152.103.161
103.62.147.2
95.181.177.197
95.181.177.197
85.254.10.197
79.180.73.190
73.21.168.251
68.45.152.8
67.232.81.208
62.205.202.214
46.70.113.73
24.210.239.177
23.240.215.189
1.29.168.152
)";

    std::stringstream stream;
    pool.print_all(stream);
    BOOST_CHECK_EQUAL(stream.str(), output);
}

BOOST_FIXTURE_TEST_CASE(test_filter_by_first_byte, Fixture)
{
    static const char* output = R"(1.29.168.152
)";

    std::stringstream stream;
    pool.print_all(stream, 1);
    BOOST_CHECK_EQUAL(stream.str(), output);
}

BOOST_FIXTURE_TEST_CASE(test_filter_by_first_and_second_byte, Fixture)
{
    static const char* output = R"(46.70.113.73
)";

    std::stringstream stream;
    pool.print_all(stream, 46, 70);
    BOOST_CHECK_EQUAL(stream.str(), output);
}

BOOST_FIXTURE_TEST_CASE(test_filter_by_any_byte, Fixture)
{
    static const char* output = R"(46.70.113.73
)";

    std::stringstream stream;
    pool.print_any(stream, 46);
    BOOST_CHECK_EQUAL(stream.str(), output);
}

}
