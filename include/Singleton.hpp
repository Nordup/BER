#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace TestBER
{
    class TcpConnection
    {
        public:
            virtual int run(std::string arg) { return 0; }

            virtual void sendData(std::string msg) {} // TODO: wrong argument by now
    };

    class Singleton
    {
    public:
        TcpConnection* connection;
        IO* input_output;
        Singleton(Singleton&) = delete;

        static Singleton& get()
        {
            static Singleton instance;
            return instance;
        }

    private:
        Singleton() {}
    };
}

#endif