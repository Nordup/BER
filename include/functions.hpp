#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "BER.hpp"

namespace TestBER
{
    /**
     * Receive data from socket in loop and handle it
     * @param socket receiveBytes from socket
     */
    void receiveDataFromSocket(StreamSocket& socket, BER& ber);

    void handleData(t_buffer buf, BER& ber);
}

#endif