#include "header.hpp"

namespace TestBER
{
#if defined(POCO_OS_FAMILY_WINDOWS)
        NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
        Event terminator;
#endif

    class TcpClient
    {
    public:

        int run(std::string arg)
        {
            try
            {
                std::string hostAndPort = (arg == "-d") ? "0.0.0.0:2001" : arg;

                connect(hostAndPort);
                std::cout << "Connected to host: " << sAddress << std::endl;
                std::cout << "Press Ctrl-C to quit." << std::endl;

                std::thread th(&TcpClient::receiveDataFromServer, this);

                terminator.wait();
                disconnect();
            }
            catch (Exception& exc)
            {
                std::cerr << exc.displayText() << std::endl;
                return 1;
            }
            return 0;
        }

    private:
        StreamSocket socket;
        SocketAddress sAddress;

        void connect(const std::string& hostAndPort)
        {
            sAddress = SocketAddress(hostAndPort);
            socket = StreamSocket();
            socket.connect(sAddress, Poco::Timespan(1l, 0l)); // sets the socket timeout and establishes a connection to the TCP server at the given address.
        }

        void disconnect()
        {
            socket.shutdown();
        }

        /**
         * Receive data and handle it
         */
        void receiveDataFromServer()
        {
           try
            {
                char buffer[256];
                int n = socket.receiveBytes(buffer, sizeof(buffer));
                while (n > 0)
                {
                    std::cout << "Received " << n << " bytes:" << std::endl;
                    std::string msg;
                    Logger::formatDump(msg, buffer, n);
                    std::cout << msg << std::endl;
                    n = socket.receiveBytes(buffer, sizeof(buffer));
                }
            }
            catch (Exception& exc)
            {
                std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
            }
        }

        //void sendMessage(std::string msg)
        //{
            //socket.sendBytes(msg.c_str(), msg.length(), 0);
        //}
    };
}

int     main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: ./client [argument]" << std::endl
            << "    argument - full Ip address or '-d' for default '0.0.0.0:2001'" << std::endl;
        return 0;
    }

    TestBER::TcpClient client;
    return client.run(argv[1]);
}