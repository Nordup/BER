#ifndef BER_HPP
#define BER_HPP

#include "header.hpp"

namespace TestBER
{
    class BER
    {
    public:
        BER();

        /**
         * Begin read new Data
         * second becouse first byte for BER tag (except: there is more than 1 byte)
         */
        std::list< std::vector<unsigned char> > readSecondByte(const t_buffer& buf, unsigned int begin);
        std::list< std::vector<unsigned char> > readLength(const t_buffer& buf, unsigned int begin);
        std::list< std::vector<unsigned char> > readValue(const t_buffer& buf, unsigned int begin);
        std::list< std::vector<unsigned char> > decodeData(const t_buffer& buf);

        static std::vector<unsigned char> encodeData(const std::list< std::vector<unsigned char> >& list);

        static std::vector<unsigned char> encodeData(const std::vector<unsigned char>& data);

    private:
        std::vector<unsigned char> storeData;
        unsigned int need_data;
        unsigned int need_length;
        bool tag;
    };
}

#endif