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
        ClientConnection(const StreamSocket& s): TCPServerConnection(s)
        {
        }

        /**
         * Receive data and handle it
         */
        void run() override
        {
            StreamSocket& ss = socket();
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
        }
    };

    typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;

    class TcpServer
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

                TestBER::terminator.wait();
            }
            catch (Exception& exc)
            {
                std::cerr << exc.displayText() << std::endl;
                return 1;
            }
            return 0;
        }
    private:

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

    TestBER::TcpServer server;
    return server.run(argv[1]);
}