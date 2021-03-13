#include "header.hpp"

namespace TestBER
{
#if defined(POCO_OS_FAMILY_WINDOWS)
    NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
    Event terminator;
#endif

    class ClientConnection: public TCPServerConnection
    {
    public:

        ClientConnection(const StreamSocket& s): TCPServerConnection(s) {}

        /**
         * Receive data and handle it
         */
        void run() override
        {
            StreamSocket& ss = socket();
            addToStaticList(ss);

            try
            {
                char buffer[256];
                int n = ss.receiveBytes(buffer, sizeof(buffer));
                while (n > 0)
                {
                    std::cout << "Received " << n << " bytes:" << std::endl;
                    std::string msg;
                    Logger::formatDump(msg, buffer, n);
                    std::cout << msg << std::endl;
                    n = ss.receiveBytes(buffer, sizeof(buffer));
                }
            }
            catch (Exception& exc)
            {
                std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
            }

            removeFromStaticList(ss);
        }

        static std::list<StreamSocket>& getClientsSockets()
        {
            // static list for all ClientConnection objects
            static std::list<StreamSocket> clientsSockets;
            return clientsSockets;
        }

        static std::mutex& getMutex()
        {
            // static mutex accessing clientsSockets
            static std::mutex mutex;
            return mutex;
        }

    private:

        /**
         * add StreamSocket to static list for accessing to send data
         */
        static void addToStaticList(StreamSocket& socket)
        {
            getMutex().lock();
            getClientsSockets().push_back(socket);
            getMutex().unlock();
        }

        /**
         * remove StreamSocket from static list
         */
        static void removeFromStaticList(StreamSocket& socket)
        {
            getMutex().lock();
            getClientsSockets().remove(socket);
            getMutex().unlock();
        }
    };

    typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;

    class Server: public TcpConnection
    {
    public:

        int run(std::string arg)
        {
            try
            {
                Poco::UInt16 port = NumberParser::parse( (arg == "-d") ? "2001" : arg );

                TCPServer srv(new TestBER::TCPFactory(), port);
                srv.start();

                std::cout << "TCP server listening on port " << port << '.'
                    << std::endl << "Press Ctrl-C to quit." << std::endl;
                
                while(true)
                {
                    sendData("Hello, client!");
                    sleep(4);
                }

                TestBER::terminator.wait();
            }
            catch (Exception& exc)
            {
                std::cerr << exc.displayText() << std::endl;
                return 1;
            }

            return 0;
        }
 
        /**
         * send data to all clients
         */
        void sendData(std::string msg) override
        {
            for (auto sSocket: ClientConnection::getClientsSockets())
            {
                try
                {
                    sSocket.sendBytes(msg.c_str(), msg.length(), 0);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
    };
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: ./server [port]" << std::endl
            << "    port - int from 0 to 65535 or '-d' for default '2001'" << std::endl;
        return 0;
    }

    TestBER::Singleton::get().connection = new TestBER::Server();
    TestBER::Singleton::get().input_output = new TestBER::IO();

    return TestBER::Singleton::get().connection->run(argv[1]);

}
