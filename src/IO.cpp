#include "IO.hpp"
#include "Singleton.hpp"

namespace TestBER
{
    IO::IO(): mutex()
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

            std::vector<unsigned char> data(message.begin(), message.end());

            std::list< std::vector<unsigned char> > list;
            list.push_back(data);

            Singleton::get().connection->sendData(list);
        }
    }
    
    void IO::printOutput(std::list< std::vector<unsigned char> > list)
    {
        for (const auto& data: list)
        {
            std::cout << std::endl << "========================================" << std::endl;

            std::cout << "Recieved " << data.size() << " bytes." << std::endl;
            std::stringstream ss;
            std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(ss, ""));

            std::cout << "String representation:" << std::endl;
            std::cout << ss.str() << std::endl;

            std::cout              << "----------------------------------------" << std::endl;
        }
    }

    void IO::addToPrint(std::list< std::vector<unsigned char> > list)
    {
        mutex.lock();
        printOutput(list);
        mutex.unlock();
    }

    IO::~IO()
    {
        alive = false;
    }
}