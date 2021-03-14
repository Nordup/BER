#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <iterator>
#include <thread>
#include <cstddef>
#include <vector>

namespace TestBER
{
    class IO
    {
    public:
        IO();
        ~IO();

        void addToPrint(std::vector<unsigned char> data);
    private:
        // while IO object is alive
        bool alive = true;

        /**
         * wait for inputs in new thread
         * and call Singleton::connection->sendData
         */
        void readInput();

        void printOutput(std::vector<unsigned char> data);
    };
}

#endif