#include "IO.hpp"
#include "Singleton.hpp"

namespace TestBER
{
    IO::IO()
    {
        std::thread read_thread(&IO::readInput, this);
        read_thread.detach();
    }

    void IO::readInput()
    {
        while (IO::alive)
        {
            std::string message;
            std::getline(std::cin, message);

            Singleton::get().connection->sendData(message);
        }
    }
    
    void IO::printOutput(std::string message)
    {
        std::cout << message << std::endl;
    }

    IO::~IO()
    {
        alive = false;
    }
}