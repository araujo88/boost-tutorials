// Binding arguments to a completion handler using class

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class Printer
{
private:
    boost::asio::steady_timer timer_;
    int count_;

public:
    Printer(boost::asio::io_context &io)
        : timer_(io, boost::asio::chrono::seconds(1)),
          count_(0)
    {
        timer_.async_wait(boost::bind(&Printer::print, this));
    }

    void print()
    {
        if (count_ < 5)
        {
            std::cout << count_ << std::endl;
            ++count_;
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1)); // adjusts time
            timer_.async_wait(boost::bind(&Printer::print, this));                // binding
        }
    }

    ~Printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }
};

int main()
{
    boost::asio::io_context io; // I/O execution context
    Printer p(io);

    std::cout << "test" << std::endl;

    io.run();

    return 0;
}