#include "header.hpp"
#include "Singleton.hpp"
#include "functions.hpp"

namespace TestBER
{
    class Client: public TcpConnection
    {
    public:
        Client(): ber() {}

        /**
         * trying to connect to the server
         * creating thread for handling incoming data
         * @return 0 if success, otherwise 1
         */
        int run(std::string arg) override
        {
            try
            {
                std::string hostAndPort = (arg == "-d") ? "0.0.0.0:2001" : arg;

                connect(hostAndPort);
                std::cout << "Connected to host: " << sAddress << std::endl;
                std::cout << "Press Ctrl-C to quit." << std::endl;

                std::thread recieve_thread(&receiveDataFromSocket, std::ref(socket), std::ref(ber));
                recieve_thread.join(); // recieve until shut down connection

                disconnect();
                std::cout << "Disconnected." << std::endl;
            }
            catch (Exception& exc)
            {
                std::cerr << exc.displayText() << std::endl;
                return 1;
            }

            return 0;
        }
        
        /**
         * send data to server
         */
        void sendData(const std::list< std::vector<unsigned char> >& vector) override
        {
            sendDataToSocket(socket, vector);
        }
        /**
         * send data to server
         */
        void sendData(const std::vector<unsigned char>& data) override
        {
            sendDataToSocket(socket, data);
        }

    private:
        StreamSocket socket;
        SocketAddress sAddress;
        BER ber;

        void connect(const std::string& hostAndPort)
        {
            sAddress = SocketAddress(hostAndPort);
            socket = StreamSocket();
            
            // set the socket timeout and establishe a connection to the TCP server at the given address.
            socket.connect(sAddress, Poco::Timespan(1l, 0l));
        }

        void disconnect()
        {
            socket.shutdown();
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

    // initialize singleton
    TestBER::Singleton::get().connection = new TestBER::Client();
    TestBER::Singleton::get().input_output = new TestBER::IO();

    // run the Client
    return TestBER::Singleton::get().connection->run(argv[1]);
}