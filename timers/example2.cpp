// using a timer asynchronously

#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code & /*e*/)
{
    std::cout << "Hello, world!" << std::endl;
}

int main()
{
    boost::asio::io_context io; // I/O execution context

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5)); // sets up timer

    t.async_wait(&print); // non-blocking wait on timer

    std::cout << "test" << std::endl;

    io.run();

    return 0;
}