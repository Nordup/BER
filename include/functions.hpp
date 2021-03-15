#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "header.hpp"
#include "BER.hpp"

namespace TestBER
{
    /**
     * Receive data from socket in loop and handle it
     * @param socket receiveBytes from socket
     */
    void receiveDataFromSocket(StreamSocket& socket, BER& ber);

    void sendDataToSocket(StreamSocket& socket, const std::list< std::vector<unsigned char> >& vector);

    void sendDataToSocket(StreamSocket& socket, const std::vector<unsigned char>& data);
}

#endif