#ifndef BER_HPP
#define BER_HPP

#include "header.hpp"

namespace TestBER
{
    class BER
    {
    public:
        std::vector<unsigned char> decodeData(t_buffer buf);

        static std::vector<unsigned char> encodeData(std::vector< std::vector<std::byte> > vertor);

    private:
        std::vector<unsigned char> fullData;
    };
}

#endif