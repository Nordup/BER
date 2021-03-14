#include "functions.hpp"
#include "Singleton.hpp"

namespace TestBER
{
    void receiveDataFromSocket(StreamSocket& socket, BER& ber)
    {
        try
        {
            t_buffer buf;
            buf.len = socket.receiveBytes(buf.data, sizeof(buf.data));
            while (buf.len > 0)
            {
                auto decoded = ber.decodeData(buf);
                if (!decoded.empty())
                    Singleton::get().input_output->addToPrint(decoded); // Singleton->IO->print

                buf.len = socket.receiveBytes(buf.data, sizeof(buf.data));
            }
        }
        catch (Exception& exc)
        {
            std::cerr << "In 'receiveDataFromSocket': " << exc.displayText() << std::endl;
        }
    }

    void sendDataToSocket(StreamSocket& socket, std::vector< std::vector<unsigned char> > vector)
    {
        try
        {
            auto encoded = BER::encodeData(vector); // TODO: move vector
            socket.sendBytes(std::data(encoded), encoded.size(), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << "In 'sendDataToSocket: " << e.what() << '\n';
        }
    }
    void sendDataToSocket(StreamSocket& socket, std::vector<unsigned char> data)
    {
        try
        {
            auto encoded = BER::encodeData(data); // TODO: move vector
            socket.sendBytes(std::data(encoded), encoded.size(), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << "In 'sendDataToSocket: " << e.what() << '\n';
        }
    }
}