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
                handleData(buf, ber);
                buf.len = socket.receiveBytes(buf.data, sizeof(buf.data));
            }
        }
        catch (Exception& exc)
        {
            std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
        }
    }

    void handleData(t_buffer buf, BER& ber)
    {
        auto data = ber.decodeData(buf);
        if (!data.empty())
            Singleton::get().input_output->addToPrint(data); // Singleton->IO->print
    }
}