
#include <array>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "client.hpp"

using boost::asio::ip::tcp;

class NetworkClient
{
public:
    NetworkClient(boost::asio::io_context& ioc, const std::string& host, const std::string& port)
        : socket_(ioc)
    {
        tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket_, endpoints);

        async_read();
    }


    void send_message(const std::string& message)
    {

        boost::asio::post(socket_.get_executor(), [this, message]()
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(message + "\n"),
                                     [](const boost::system::error_code& ec, std::size_t bytes)
                                     {
                                         if (!ec)
                                             std::cout << "Sent " << bytes << " bytes.\n";
                                         else
                                             std::cerr << "Write error: " << ec.message() << std::endl;
                                     });
        });
    }

private:
    void async_read()
    {

        auto buf = std::make_shared<boost::asio::streambuf>();

        boost::asio::async_read_until(
            socket_, *buf, '\n',
            [this, buf](const boost::system::error_code &error, std::size_t /*bytes_transferred*/)
            {
                if (!error)
                {
                    std::istream is(buf.get());
                    std::string line;
                    std::getline(is, line);

                    std::cout << "Received: " << line << std::endl;


                    async_read();
                }
                else
                {
                    std::cerr << "Read error: " << error.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
};