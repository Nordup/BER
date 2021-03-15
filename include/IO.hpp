#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <sstream>

namespace TestBER
{
    class IO
    {
    public:
        /**
         * run readInput() tread detached
         */
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