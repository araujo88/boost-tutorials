#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

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
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

        while (true)
        {
            tcp::socket socket(io_context);
            boost::system::error_code error;

            acceptor.accept(socket);

            std::string message = make_daytime_string();

            boost::asio::write(socket, boost::asio::buffer(message), error);

            std::cout << message << std::endl;

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