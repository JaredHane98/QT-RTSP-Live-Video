#ifndef TCP_MANAGER_HPP
#define TCP_MANAGER_HPP
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <memory.h>
#include <stdint.h>
#include "ModuleController.hpp"


using boost::asio::ip::tcp;

class TCPManagerClient
{
private:
    std::shared_ptr<Module> m_module;
    std::string m_server_ip;
    std::string m_server_port;

    boost::asio::io_service m_service; 
    std::shared_ptr<tcp::socket> m_socket;
    std::shared_ptr<tcp::resolver> m_resolver;
public:
    TCPManagerClient(const std::string& server_ip, const std::string& server_port);

    ~TCPManagerClient();

    TCPManagerClient(const TCPManagerClient&) = delete; 
    TCPManagerClient(TCPManagerClient&&) = delete;
private:
    boost::system::error_code attemptToConnect();

    boost::system::error_code shutdownSocket();

    boost::system::error_code closeSocket(); 

    void waitForConnection(); 

    bool isOpen();

    uint64_t write(const void* buffer, const uint64_t buffer_size, boost::system::error_code& ec);

    uint64_t read(void* buffer, const uint64_t buffer_size, boost::system::error_code& ec);     

    uint64_t readClient(void* buffer, const uint64_t buffer_size);

    uint64_t writeClient(void* buffer, const uint64_t num_bytes);

    void runMainLoop();

    static void run(const std::string& server_ip, const std::string& server_port);
public:
    static void start(const std::string& server_ip, const std::string& server_port);
}; 



#endif // TCP_MANAGER_HPP
