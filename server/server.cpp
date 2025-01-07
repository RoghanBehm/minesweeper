#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_server;

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    using pointer = std::shared_ptr<tcp_connection>;

    static pointer create(tcp_server& server, boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(server, io_context));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        do_read();
    }

    void send_message(const std::string& message)
    {
        auto self = shared_from_this();
        std::string formatted_message = message + "\n";
        boost::asio::async_write(socket_, boost::asio::buffer(formatted_message),
            [this, self](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
                if (error)
                {
                    std::cout << "Error sending coordinates: " << error.message() << std::endl;
                }
            });
    }

private:
    tcp_connection(tcp_server& server, boost::asio::io_context& io_context)
        : server_(server), socket_(io_context)
    {
    }

    void do_read();

    tcp_server& server_;
    tcp::socket socket_;
    std::array<char, 128> buffer_;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 8000))
    {
        start_accept();
    }

    void broadcast_message(const std::string& message, std::shared_ptr<tcp_connection> sender)
    {
        for (auto& client : clients_)
        {
            if (client != sender)
            {
                client->send_message(message);
            }
        }
    }

    void add_client(std::shared_ptr<tcp_connection> client)
    {
        clients_.push_back(client);
    }

    void remove_client(std::shared_ptr<tcp_connection> client)
    {
        clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
    }

private:
    void start_accept()
    {
        auto new_connection = tcp_connection::create(*this, io_context_);

        acceptor_.async_accept(new_connection->socket(),
            [this, new_connection](const boost::system::error_code& error) {
                if (!error)
                {
                    std::cout << "New client connected!" << std::endl;
                    add_client(new_connection);
                    new_connection->start();
                }
                start_accept();
            });
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<tcp_connection>> clients_;
};

void tcp_connection::do_read()
{
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_),
        [this, self](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error)
            {
                std::string message(buffer_.data(), bytes_transferred);
                std::cout << "Received: " << message << std::endl;

                server_.broadcast_message(message, self);

                do_read();
            }
            else
            {
                std::cout << "Client disconnected." << std::endl;
                server_.remove_client(self);
            }
        });
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp_server server(io_context);

        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
