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

        ClientConnection();
        ClientConnection(const StreamSocket& s): TCPServerConnection(s) {}

        /**
         * Receive data and handle it
         */
        void run() override
        {
            StreamSocket& ss = socket();
            // store socket in static list field
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

        static std::list<StreamSocket> getClientsSockets()
        {
            return clientsSockets;
        }

    private:
        static std::list<StreamSocket> clientsSockets;
        static std::mutex mutex;

        static void addToStaticList(StreamSocket& socket)
        {
            mutex.lock();
            clientsSockets.push_back(socket);
            mutex.unlock();
        }

        static void removeFromStaticList(StreamSocket& socket)
        {
            mutex.lock();
            clientsSockets.remove(socket);
            mutex.unlock();
        }
    };

    typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;

    class Server: TcpConnection
    {
    public:

        int run(std::string arg)
        {
            try
            {
                Poco::UInt16 port = NumberParser::parse( (arg == "-d") ? "2001" : arg );

                TCPServer srv(new TestBER::TCPFactory(), port);
                srv.start();
                //tcpServer = &srv;

                std::cout << "TCP server listening on port " << port << '.'
                    << std::endl << "Press Ctrl-C to quit." << std::endl;
                
                while(true)
                {
                    sendDataToAll("Hello, client!");
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

   private:
        //TCPServer *tcpServer; // pointer to stack allocated variable
    };
}

// store StreamSockets
std::list<StreamSocket> TestBER::ClientConnection::clientsSockets;
std::mutex TestBER::ClientConnection::mutex;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: ./server [port]" << std::endl
            << "    port - int from 0 to 65535 or '-d' for default '2001'" << std::endl;
        return 0;
    }

    TestBER::Server server;
    return server.run(argv[1]);
}
