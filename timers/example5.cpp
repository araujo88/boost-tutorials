// Synchronising completion handlers in multithreaded programs

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>

class Printer
{
private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;

public:
    Printer(boost::asio::io_context &io) : strand_(boost::asio::make_strand(io)),
                                           timer1_(io, boost::asio::chrono::seconds(1)),
                                           timer2_(io, boost::asio::chrono::seconds(1)),
                                           count_(0)
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_,
                                                      boost::bind(&Printer::print1, this)));

        timer2_.async_wait(boost::asio::bind_executor(strand_,
                                                      boost::bind(&Printer::print2, this)));
    }
    void print1()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

            timer1_.async_wait(boost::asio::bind_executor(strand_,
                                                          boost::bind(&Printer::print1, this)));
        }
    }

    void print2()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

            timer2_.async_wait(boost::asio::bind_executor(strand_,
                                                          boost::bind(&Printer::print2, this)));
        }
    }

    ~Printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }
};

int main()
{
    boost::asio::io_context io;
    Printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();

    return 0;
}