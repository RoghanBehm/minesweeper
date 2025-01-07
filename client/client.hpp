#include <boost/asio.hpp>

class NetworkClient
{
public:
        NetworkClient(boost::asio::io_context& ioc, const std::string& host, const std::string& port);
        void send_message(const std::string& message);
private:
    void async_read();
};