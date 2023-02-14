// using a timer synchronously

#include <iostream>
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_context io; // I/O execution context

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5)); // sets up timer

    t.wait(); // blocking wait on timer

    std::cout << "Hello, world!" << std::endl;

    return 0;
}