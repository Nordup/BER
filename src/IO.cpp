#include "IO.hpp"
#include "Singleton.hpp"
#include "test.hpp"

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

            std::vector<unsigned char> data(message.begin(), message.end());

            std::vector< std::vector<unsigned char> > vector;
            vector.push_back(data);

            //test
            //std::cout << "Send data:" << std::endl;
            //test_print_Vector(data);

            //Singleton::get().connection->sendData(data);
            Singleton::get().connection->sendData(vector);
        }
    }
    
    void IO::printOutput(std::vector<unsigned char> data)
    {
        std::cout << "Recieved " << data.size() << " bytes." << std::endl;

        std::cout << "String representation:" << std::endl;

        std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(std::cout, ""));
        std::cout << std::endl;
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