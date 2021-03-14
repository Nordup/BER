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
    
    void IO::printOutput(std::vector<unsigned char> data)
    {
        std::cout << "Recieved " << data.size() << " bytes." << std::endl;

        std::cout << "String representation:" << std::endl;

        std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(std::cout, ""));
    }

    void IO::addToPrint(std::vector<unsigned char> data)
    {
        // by now
        printOutput(data);
    }

    IO::~IO()
    {
        alive = false;
    }
}