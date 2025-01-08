
#include <array>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "client.hpp"
#include "../include/serialize.hpp"

using boost::asio::ip::tcp;


    NetworkClient::NetworkClient(boost::asio::io_context& ioc, const std::string& host, const std::string& port)
        : socket_(ioc)
    {
        tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket_, endpoints);

        async_read();
    }


    void NetworkClient::send_message(const std::vector<char>& message)
    {

        boost::asio::post(socket_.get_executor(), [this, message]()
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(message),
                                     [](const boost::system::error_code& ec, std::size_t bytes)
                                     {
                                         if (!ec)
                                             std::cout << "Sent " << bytes << " bytes.\n";
                                         else
                                             std::cerr << "Write error: " << ec.message() << std::endl;
                                     });
        });
    }


    void NetworkClient::async_read()
{
    auto buffer = std::make_shared<std::vector<char>>(128);

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(*buffer),
        [this, buffer](const boost::system::error_code& ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                buffer->resize(bytes_transferred);
                auto coords = deserialize_pairs(*buffer);

                if (!coords.empty()) {
                    for (auto& [x, y] : coords)
                        std::cout << "(" << x << ", " << y << ") ";
                    std::cout << std::endl;
                }

                async_read();
            }
            else
            {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
        }
    );
}


