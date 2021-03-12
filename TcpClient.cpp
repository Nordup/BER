#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <iostream>

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


namespace
{
    class TcpClient: public Poco::Runnable
    {
    public:
        void connect(const std::string& hostAndPort)
        {
            //try
            //{
                sAddress = SocketAddress(hostAndPort);
                socket = StreamSocket();
                socket.connect(sAddress, Poco::Timespan(1l, 0l)); // sets the socket timeout and establishes a connection to the TCP server at the given address.
            //}
            //catch(Exception& ex)
            //{
                //std::cerr << "Connection: " << ex.displayText() << std::endl;
            //}
        }

        void disconnect()
        {
            socket.shutdown();
        }

        void run() override
        {
            // send and recieve data
        }

        std::string getIpAddress()
        {
            return sAddress.toString();
        }

    private:
        StreamSocket socket;
        SocketAddress sAddress;
    };

#if defined(POCO_OS_FAMILY_WINDOWS)
        NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
        Event terminator;
#endif
}

int     main(int argc, char** argv)
{
    TcpClient client;

    if (argc != 2)
    {
        std::cout << "usage: ./client [argument]" << std::endl
            << "    argument - full Ip address or '-d' for default '0.0.0.0:2001'" << std::endl;
        return 0;
    }

    try
    {
        client.connect( (std::strcmp(argv[1], "-d") == 0) ? "0.0.0.0:2001" : argv[1] );
        std::cout << "Connected to host: " << client.getIpAddress() << std::endl;
        
        terminator.wait();
    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }
    
    return 0;
}