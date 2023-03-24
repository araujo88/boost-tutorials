#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
private:
    TcpConnection(boost::asio::io_context &io_context) : socket_(io_context)
    {
    }

    void handle_write(const boost::system::error_code &, size_t)
    {
    }

    tcp::socket socket_;
    std::string message_;

public:
    typedef boost::shared_ptr<TcpConnection> pointer;

    static pointer create(boost::asio::io_context &io_context)
    {
        return pointer(new TcpConnection(io_context));
    }

    tcp::socket &socket()
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(message_),
                                 boost::bind(&TcpConnection::handle_write, shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }
};

class TcpServer
{
private:
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;

public:
    TcpServer(boost::asio::io_context &io_context) : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
    {
        start_accept();
    }

    void start_accept()
    {
        TcpConnection::pointer new_connection = TcpConnection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&TcpServer::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(TcpConnection::pointer new_connection, const boost::system::error_code &error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }
};

int main(int argc, char **argv)
{

    try
    {
        boost::asio::io_context io_context;
        TcpServer server(io_context);
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}