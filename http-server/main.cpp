#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

void handle_request(http::request<http::string_body> &request, http::response<http::string_body> &response)
{
    std::cout << request.method_string() << " "
              << request.target() << " " << request.body() << std::endl;

    // Here you would dispatch based on the URI and HTTP method
    if ((request.method_string()) == "GET" && (request.target() == "/"))
    {
        response.result(http::status::ok);
        response.body() = "Hello, world!";
    }
    else if (request.method_string() != "GET")
    {
        response.result(http::status::method_not_allowed);
        response.body() = "Method not allowed";
    }
    else
    {
        response.result(http::status::ok);
        response.body() = request.target().substr(1);
    }
    response.prepare_payload();
}

void session(tcp::socket socket)
{
    try
    {
        beast::flat_buffer buffer;

        // Set up an HTTP request parser
        http::request<http::string_body> request;
        http::read(socket, buffer, request);

        // Handle the request
        http::response<http::string_body> response;
        handle_request(request, response);

        // Send the response
        http::write(socket, response);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void run_server(short port)
{
    net::io_context io_context{1};

    tcp::acceptor acceptor{io_context, {tcp::v4(), static_cast<boost::asio::ip::port_type>(port)}};
    for (;;)
    {
        tcp::socket socket{io_context};
        acceptor.accept(socket);

        // Handle each session in a separate thread
        std::thread{session, std::move(socket)}.detach();
    }
}

int main()
{
    try
    {
        short port = 6969;
        std::cout << "Server starting on port " << port << std::endl;
        run_server(port);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
