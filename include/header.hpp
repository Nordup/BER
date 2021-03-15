#ifndef HEADER_HPP
#define HEADER_HPP

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"

// std libraries
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>

using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFilter;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::UInt16;
using Poco::NumberParser;
using Poco::Logger;
using Poco::Event;
using Poco::NamedEvent;
using Poco::Process;
using Poco::ProcessImpl;
using Poco::Exception;

namespace TestBER
{
    #define BUFFER_SIZE 256

    typedef struct s_byteBuffer
    {
        unsigned char data[BUFFER_SIZE];
        unsigned int len;
    } t_buffer;
}

#endif