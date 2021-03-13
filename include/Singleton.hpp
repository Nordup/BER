#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace TestBER
{
    class TcpConnection
    {
        public:
            virtual int run(std::string arg);
            virtual void sendData(std::string msg); // TODO: wrong argument by now
    };

    class Singleton
    {

    };
}

#endif