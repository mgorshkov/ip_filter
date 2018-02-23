#include <tuple>
#include <map>
#include <vector>
#include <functional>

using IpOctetType = int;
using IpAddr = std::tuple<IpOctetType, IpOctetType, IpOctetType, IpOctetType>;

extern bool Compare(int value, int data);

template <typename Type, typename... Types>
bool FindInTuple(const std::tuple<Types...>& tuple, const Type& value)
{
    auto Find = [&value](auto&&... args)
    {
        return (Compare(value, decltype(args)(args)) || ...);
    };
    return std::apply(Find, tuple);
}

template <size_t I>
struct CompareTuples
{
    template <typename... Types1, typename... Types2>
    static constexpr bool CompareAll(const std::tuple<Types1...>& data, const std::tuple<Types2...>& pattern)
    {
        if (std::get<I - 1>(data) != std::get<I - 1>(pattern))
            return false;

        return CompareTuples<I - 1>::CompareAll(data, pattern);
    }

    template <typename... Types1, typename... Types2>
    static constexpr bool CompareAny(const std::tuple<Types1...>& data, const std::tuple<Types2...>& pattern)
    {
        if (FindInTuple(data, std::get<I - 1>(pattern)))
             return true;

        return CompareTuples<I - 1>::CompareAny(data, pattern);
    }
};

template <>
struct CompareTuples<0>
{
    template <typename... Types1, typename... Types2>
    static constexpr bool CompareAll(const std::tuple<Types1...>&, const std::tuple<Types2...>&)
    {
        return true;
    }

    template <typename... Types1, typename... Types2>
    static constexpr bool CompareAny(const std::tuple<Types1...>&, const std::tuple<Types2...>&)
    {
        return false;
    }
};

template <typename... Types, size_t... I>
constexpr auto MakeTupleFromVectorHelper(const std::vector<std::string>& v, std::index_sequence<I...>)
{
    return std::make_tuple(atoi(v[I].c_str()) ...);
}

template <typename>
struct MakeTupleFromVector;

template <typename... Types>
struct MakeTupleFromVector<std::tuple<Types...>>
{
    static auto Make(const std::vector<std::string>& v)
    {
        return MakeTupleFromVectorHelper(v, std::make_index_sequence<sizeof... (Types)>());
    }
};

template <typename TupType, size_t... I>
void PrintTuple(std::ostream& ostream, const TupType& tuple, std::index_sequence<I...>)
{
    (..., (ostream << std::string(I == 0 ? "" : ".") << std::get<I>(tuple)));
}

template <typename... Types>
void PrintTuple(std::ostream& ostream, const std::tuple<Types...>& tuple)
{
    PrintTuple(ostream, tuple, std::make_index_sequence<sizeof...(Types)>());
}

class IpAddrPool
{
public:
    void insert(const IpAddr& addr);
    void insert(const std::string& str);

    template <typename... Args>
    void print_all(std::ostream& ostream, Args&&... args)
    {
        auto filterAll = [&](const auto& addr)
        {
            return CompareTuples<sizeof...(args)>::CompareAll(addr, std::make_tuple(args...));
        };
        print_filtered(ostream, filterAll);
    }

    template <typename... Args>
    void print_any(std::ostream& ostream, Args&&... args)
    {
        auto filterAny = [&](const auto& addr)
        {
            return CompareTuples<sizeof...(args)>::CompareAny(addr, std::make_tuple(args...));
        };
        print_filtered(ostream, filterAny);
    }

private:
    void print_filtered(std::ostream& ostream, std::function<bool(IpAddr)> filter);

    using IpAddrPoolContainer = std::map<IpAddr, int, std::greater<IpAddr>>;

    IpAddrPoolContainer container;
};

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d);

int version();
