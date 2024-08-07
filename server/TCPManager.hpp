#ifndef TCP_MANAGER_HPP
#define TCP_MANAGER_HPP
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using boost::asio::ip::tcp;

class TCPManagerServer
{
private:
    boost::asio::io_service m_service;
    std::shared_ptr<tcp::acceptor> m_acceptor;
    std::shared_ptr<tcp::socket> m_socket;
    const int16_t m_port;
public:
    TCPManagerServer(const int16_t port);

    ~TCPManagerServer();

    TCPManagerServer(const TCPManagerServer&) = delete;
    TCPManagerServer(TCPManagerServer&&) = delete;
private:
    bool isOpen();

    boost::system::error_code shutdownSocket();

    boost::system::error_code closeSocket();

    boost::system::error_code acceptConnection();

    uint64_t read(void* buffer, const uint64_t buffer_size, boost::system::error_code& ec);

    uint64_t write(const void* buffer, const uint64_t buffer_size, boost::system::error_code& ec);

    void waitForConnection();

    uint64_t writeServer(const void* buffer, const uint64_t buffer_size);

    uint64_t readServer(void* buf, const uint64_t num_bytes);

    void runMainLoop();

    void readAck();

    static void run(const uint16_t port);
public:
    static void start(const uint16_t port);
};







#endif // TCP_MANAGER_HPP






