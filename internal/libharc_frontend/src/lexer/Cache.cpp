#include <harc/lexer/Cache.hpp>

namespace harc::lex {



    Cache parse_binary_cache(std::span<const std::byte> bytes) {
        //TODO: implement
        return Cache{};
    }

    std::vector<std::byte> serialize_cache(const Cache& cache) {
        std::size_t ret_size = 0;


        std::vector<std::byte> ret;
        ret.resize(ret_size);
        //TODO: implement
        return {};
    }

    Cache parse_text_cache(std::string_view cahce) {
        Cache ret;

        

        return ret;
    }

}
