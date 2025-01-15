#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <regex>
#include "serverIP.hpp"

std::string getPublicIPviaBeast()
{
    try {
        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net  = boost::asio;
        using tcp      = net::ip::tcp;

        net::io_context ioc;

        tcp::resolver resolver(ioc);
        tcp::socket socket(ioc);

        auto const results = resolver.resolve("ifconfig.me", "80");

        net::connect(socket, results.begin(), results.end());

        http::request<http::string_body> req(http::verb::get, "/", 11 /*HTTP/1.1*/);
        req.set(http::field::host, "ifconfig.me");
        req.set(http::field::user_agent, "MyMinesweeperGame/1.0");

        http::write(socket, req);

        beast::flat_buffer buffer;

        http::response<http::dynamic_body> res;

        http::read(socket, buffer, res);

        beast::error_code ec;
        socket.shutdown(tcp::socket::shutdown_both, ec);

        std::string bodyStr = beast::buffers_to_string(res.body().data());
        std::string ip;
        std::string port;

        std::regex ipReg(R"(ip_addr:\s*(\d+\.\d+\.\d+\.\d+))");
        std::regex portReg(R"(port:\s*(\d+))");

        std::smatch ipMatch, portMatch;

        if (std::regex_search(bodyStr, ipMatch, ipReg) && ipMatch.size() > 1) {
            ip = ipMatch[1].str();
        }

        if (std::regex_search(bodyStr, portMatch, portReg) && portMatch.size() > 1) {
            port = portMatch[1].str();
        } else {
            std::cout << "Could not find port.\n";
        }
        
        return "Your public IP is: " + ip + ":" + port;
    }
    catch (const std::exception &e) {
        std::cerr << "Error in getPublicIPviaBeast: " << e.what() << std::endl;
        return {};
    }
}
