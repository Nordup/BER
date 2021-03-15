#include "BER.hpp"

namespace TestBER
{
    BER::BER(): storeData()
    {
        need_data = 0;
        need_length = 0;
    }

    std::list< std::vector<unsigned char> > BER::readSecondByte(const t_buffer& buf, unsigned int begin)
    {
        // clear vector
        storeData.clear();
        storeData.resize(0);

        if (buf.len - begin >= 2)
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
            return {};
        }
    }

    std::list< std::vector<unsigned char> > BER::readLength(const t_buffer& buf, unsigned int begin)
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

    std::list< std::vector<unsigned char> > BER::readValue(const t_buffer& buf, unsigned int begin)
    {
        unsigned int data_len = buf.len - begin;

        if (need_data == 0)
            return {storeData};
        else if (data_len == 0)
            return {};

        if (data_len < need_data)
        {
            // insert bufer from begin to end
            storeData.insert(storeData.end(), buf.data + begin, buf.data + buf.len);

            need_data -= data_len;
            return {};
        }
        else if (data_len == need_data)
        {
            // insert bufer from begin to end
            storeData.insert(storeData.end(), buf.data + begin, buf.data + buf.len);

            need_data = 0;
            return {storeData};
        }
        else // data_len > need_data
        {
            // insert need_data length
            storeData.insert(storeData.end(), buf.data + begin, buf.data + begin + need_data);

            // becouse second call may erase this data
            auto fullData = std::move(storeData);

            // recursively read next data structure
            auto list = readSecondByte(buf, begin + need_data); // be sure to increase begin arg
            list.push_front(fullData);

            need_data = 0;
            return list;
        }
    }

    std::list< std::vector<unsigned char> > BER::decodeData(const t_buffer& buf)
    {
        if (buf.len <= 0)
            return {};

        if (need_data == 0 && need_length == 0) // new data
        {
            return readSecondByte(buf, 0);
        }
        else if (need_data == 0) // need_data length is not counted at all
        {
            return readLength(buf, 0);
        }
        else if (need_length == 0) // need_data is counted, need to read Value
        {
            return readValue(buf, 0);
        }
        else // need_data length not fully counted
        {
            return readLength(buf, 0);
        }
    }

    std::vector<unsigned char> BER::encodeData(const std::list< std::vector<unsigned char> >& list)
    {
        std::vector<unsigned char> fullData;

        for (auto& data: list)
        {
            fullData.insert(fullData.end(), data.begin(), data.end());
        }
        return fullData;
    }

    std::vector<unsigned char> BER::encodeData(const std::vector<unsigned char>& data)
    {
        return data;
    }
}