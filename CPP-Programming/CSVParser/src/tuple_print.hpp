#include <cstddef>
#include <iostream>
#include <tuple>

template<size_t index, typename Ch, typename Tr, typename ...Types>
void print_tuple(std::basic_ostream<Ch, Tr>& output, std::tuple<Types...> tuple){
    if constexpr (index < sizeof...(Types) - 1){
        output << std::get<index>(tuple) << ' ';
        print_tuple<index + 1, Ch, Tr, Types...>(output, tuple);
    }
    else{
        output << std::get<index>(tuple);
    }
}

template<typename Ch, typename Tr, typename ...Types>
auto operator<<(std::basic_ostream<Ch, Tr>& output, std::tuple<Types...> const& tuple){
    print_tuple<0, Ch, Tr, Types...>(output, tuple);
    output << '\n';
}
