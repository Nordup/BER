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
            /**
             * Client.run() or Server.run()
             */
            virtual int run(std::string arg) = 0;

            virtual void sendData(const std::list< std::vector<unsigned char> >& vector) = 0;
            virtual void sendData(const std::vector<unsigned char>& data) = 0;
    };

    // for Server->IO / IO->Server
    // and Client->IO / IO->Client
    // interaction
    class Singleton
    {
    public:
        TcpConnection* connection = NULL; // TcpConnection: Server or Client object
        IO* input_output = NULL; // IO for input output

        Singleton(Singleton&) = delete;

        /**
         * @return static instance of Singleton
         */
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