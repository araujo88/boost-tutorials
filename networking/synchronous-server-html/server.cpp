#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std;

    time_t now = time(0);
    return ctime(&now);
}

int main(int argc, char **argv)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8000));

        while (true)
        {
            tcp::socket socket(io_context);
            boost::system::error_code error;

            acceptor.accept(socket);

            std::string message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world!</h1></body></html>";

            boost::asio::write(socket, boost::asio::buffer(message), error);

            boost::array<char, 1024> buf;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            std::cout.write(buf.data(), len);

            if (error)
                throw boost::system::system_error(error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}