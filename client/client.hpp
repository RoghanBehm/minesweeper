#pragma once
#include <boost/asio.hpp>

class NetworkClient
{
public:
        NetworkClient(boost::asio::io_context& ioc, const std::string& host, const std::string& port);
            void send_message(const std::vector<char>& message);
            void read_seed();
private:
    void async_read();
    boost::asio::ip::tcp::socket socket_;
    
};