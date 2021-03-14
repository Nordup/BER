#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <thread>

namespace TestBER
{
    class IO
    {
    public:
        IO();
        ~IO();

        void printOutput(std::string message);
    private:
        // while IO object is alive
        bool alive = true;

        /**
         * wait for inputs in new thread
         * and call Singleton::connection->sendData
         */
        void readInput();
    };
}

#endif