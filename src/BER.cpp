#include "BER.hpp"

namespace TestBER
{
    std::vector<unsigned char> BER::decodeData(t_buffer buf)
    {
        std::vector<unsigned char> data(buf.data, buf.data + buf.len);
        return data;
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