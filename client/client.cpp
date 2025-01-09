
#include <array>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "client.hpp"
#include "../include/serialize.hpp"
#include "../include/settings.hpp"

using boost::asio::ip::tcp;

NetworkClient::NetworkClient(boost::asio::io_context &ioc, const std::string &host, const std::string &port)
    : socket_(ioc)
{
    tcp::resolver resolver(ioc);
    auto endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket_, endpoints);

    read_seed();
}

void NetworkClient::send_message(const std::vector<char> &message)
{

    boost::asio::post(socket_.get_executor(), [this, message]()
                      { boost::asio::async_write(socket_,
                            boost::asio::buffer(message),
                            [](const boost::system::error_code &ec, std::size_t bytes)
                            {
                                if (!ec)
                                    std::cout << "Sent " << bytes << " bytes.\n";
                                else
                                    std::cerr << "Write error: " << ec.message() << std::endl;
                            }); });
}

void NetworkClient::read_seed()
{
    auto buffer = std::make_shared<std::vector<char>>(8);

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(*buffer),
        boost::asio::transfer_exactly(8),
        [this, buffer](const boost::system::error_code &ec, std::size_t bytes_transferred)
        {
            if (!ec && bytes_transferred == 8)
            {
                MessageType type;
                std::memcpy(&type, buffer->data(), sizeof(MessageType));

                if (type == MessageType::Seed)
                {
                    int seed;
                    std::memcpy(&seed,
                                buffer->data() + sizeof(MessageType),
                                sizeof(seed));

                    std::cout << "Received seed: " << seed << std::endl;
                    std::srand(seed);
                }
                else
                {
                    std::cerr << "Expected Seed message, got something else.\n";
                }

                if (!ec && bytes_transferred == 8)
                {
                    globalSettings.seed_received = true;
                    async_read();
                }
                
            }
            else
            {
                std::cerr << "Read error (seed): " << ec.message() << std::endl;
            }
        });
}

void NetworkClient::async_read()
{
    auto buffer = std::make_shared<std::vector<char>>(128);
    socket_.async_read_some(
        boost::asio::buffer(*buffer),
        [this, buffer](const boost::system::error_code &ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                buffer->resize(bytes_transferred);

                if (bytes_transferred >= sizeof(MessageType))
                {
                    MessageType type;
                    std::memcpy(&type, buffer->data(), sizeof(MessageType));

                    if (type == MessageType::Coordinates)
                    {
                        auto coords = deserialize_pairs(
                            std::vector<char>(buffer->begin() + sizeof(MessageType),
                                              buffer->end()));

                        if (!coords.empty())
                        {
                            for (auto &[x, y] : coords)
                                std::cout << "(" << x << ", " << y << ") ";
                            std::cout << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Got a non-coordinate message type :-|\n";
                    }
                }
                async_read();
            }
            else
            {
                std::cerr << "Read error (coords): " << ec.message() << std::endl;
            }
        });
}

