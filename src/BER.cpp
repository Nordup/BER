#include "BER.hpp"

namespace TestBER
{
    BER::BER(): storeData()
    {
        need_data = 0;
        need_length = 0;
    }

    std::vector<unsigned char> BER::readSecondByte(const t_buffer& buf, unsigned int begin)
    {
        // clear vector
        storeData.resize(0);
        storeData.clear();

        if (buf.len >= 2)
        {
            unsigned char length_first = buf.data[begin + 1]; // +1 for second
            unsigned int _need = length_first & (unsigned char)127; // 127 = 0111 1111

            if ( (length_first >> 7) == 0 ) // shift right 7 bits
            {
                need_data = _need;
                need_length = 0; // for sure

                // debug
                std::cout << "debug; need_data: " << need_data << std::endl;

                storeData.resize(need_data); // size is counted
                return readValue(buf, begin + 2);
            }
            else
            {
                need_data = 0; // for sure
                need_length = _need;

                return readLength(buf, begin + 2);
            }
        }
        else
        {
            std::cout << "Received wrong message. Size must be more than 1 byte." << std::endl;
        }
    }

    std::vector<unsigned char> BER::readLength(const t_buffer& buf, unsigned int begin)
    {
        if (buf.len >= need_length + begin)
        {
            for (int i = begin; i < need_length + begin; i++)
            {
                need_data <<= 8; // shift left byte
                need_data += buf.data[i];
            }

            begin += need_length; // set next begin point
            need_length = 0;

            storeData.resize(need_data); // size is counted
            return readValue(buf, begin);
        }
        else
        {
            for (int i = begin; i < buf.len; i++)
            {
                need_data <<= 8; // shift left byte
                need_data += buf.data[i];
            }

            need_length -= (buf.len - begin); // update status: need_length > 0
            return {}; // empty vector
        }
    }

    std::vector<unsigned char> BER::readValue(const t_buffer& buf, unsigned int begin)
    {
        if (buf.len < need_data)
        {

        }
        else if (buf.len == need_data)
        {

        }
        else
        {

        }
    }

    std::vector<unsigned char> BER::decodeData(const t_buffer& buf)
    {
        if (buf.len <= 0)
            return {};

        if (need_data == 0 && need_length == 0) // new data
        {
            readSecondByte(buf, 0);
        }
        else if (need_data == 0) // need_data length is not counted at all
        {
            readLength(buf, 0);
        }
        else if (need_length == 0) // need_data is counted, need to read Value
        {
            readValue(buf, 0);
        }
        else // need_data length not fully counted
        {
            readLength(buf, 0);
        }

        return {};
    }

    std::vector<unsigned char> BER::encodeData(std::vector< std::vector<unsigned char> > vector)
    {
        std::vector<unsigned char> fullData;

        for (auto& data: vector)
        {
            fullData.insert(fullData.end(), data.begin(), data.end());
        }
        return fullData;
    }

    std::vector<unsigned char> BER::encodeData(std::vector<unsigned char> data)
    {
        return data;
    }
}