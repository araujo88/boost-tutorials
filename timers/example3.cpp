// Binding arguments to a completion handler

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

void print(const boost::system::error_code & /*e*/, boost::asio::steady_timer *t, int *count)
{
    if (*count < 5)
    {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));                  // adjusts time
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count)); // binding
    }
}

int main()
{
    boost::asio::io_context io;                                       // I/O execution context
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1)); // sets up timer
    int count = 0;

    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count)); // non-blocking wait on timer

    std::cout << "test" << std::endl;

    io.run();

    std::cout << "Final count is " << count << std::endl;

    return 0;
}