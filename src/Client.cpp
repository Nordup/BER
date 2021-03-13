#include "header.hpp"

namespace TestBER
{
    class Client: public TcpConnection
    {
    public:

        int run(std::string arg) override
        {
            try
            {
                std::string hostAndPort = (arg == "-d") ? "0.0.0.0:2001" : arg;

                connect(hostAndPort);
                std::cout << "Connected to host: " << sAddress << std::endl;
                std::cout << "Press Ctrl-C to quit." << std::endl;

                std::thread recieve_thread(&Client::receiveDataFromServer, this);
                recieve_thread.join(); // recieve until shut down connection

                disconnect();
            }
            catch (Exception& exc)
            {
                std::cerr << exc.displayText() << std::endl;
                return 1;
            }

            return 0;
        }
        
        void sendData(std::string msg) override
        {
            socket.sendBytes(msg.c_str(), msg.length(), 0);
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

    TestBER::Singleton::get().connection = new TestBER::Client();
    TestBER::Singleton::get().input_output = new TestBER::IO();

    return TestBER::Singleton::get().connection->run(argv[1]);
}