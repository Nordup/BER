#ifndef BER_HPP
#define BER_HPP

#include "header.hpp"

namespace TestBER
{
    class BER
    {
    public:
        std::vector<unsigned char> decodeData(t_buffer buf);

        static std::vector<unsigned char> encodeData(std::vector< std::vector<unsigned char> > vector);

        static std::vector<unsigned char> encodeData(std::vector<unsigned char> data);

    private:
        std::vector<unsigned char> storeData;
    };
}

#endif