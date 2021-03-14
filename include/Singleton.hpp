#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "IO.hpp"

namespace TestBER
{
    // interface for Server and Client
    // for handling from common Singleton class
    class TcpConnection
    {
        public:
            virtual int run(std::string arg) = 0;

            virtual void sendData(std::vector< std::vector<unsigned char> > vector) = 0;

            virtual void sendData(std::vector<unsigned char> data) = 0;
    };

    // for Server->IO / IO->Server
    // and Client->IO / IO->Client
    // interaction
    class Singleton
    {
    public:
        TcpConnection* connection = NULL;
        IO* input_output = NULL;

        Singleton(Singleton&) = delete;

        static Singleton& get()
        {
            static Singleton instance;
            return instance;
        }

       ~Singleton()
        {
            delete connection;
            delete input_output;
        }

    private:
        Singleton() {}
    };
}

#endif