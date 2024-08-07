#include "TCPManager.hpp"
#include "ModuleController.hpp"
#include <boost/asio/completion_condition.hpp>
#include <thread>
#include <chrono>
#include <iostream> 



TCPManagerClient::TCPManagerClient(const std::string& server_ip, const std::string& server_port)
    : m_module(ModuleController::createModule("TCP_CLIENT")), m_server_ip(server_ip), 
      m_server_port(server_port), m_service(), m_socket(), m_resolver()
{
    m_module->addInputValue("DISCONNECTED", "STATUS");
}

TCPManagerClient::~TCPManagerClient()
{
    if(isOpen())
    {
        boost::system::error_code shutdown = shutdownSocket();
        if(shutdown)
            m_module->appendInputValue(shutdown.message(), "TCP_LOG");
        else
        {
            boost::system::error_code socket_ec = closeSocket();
            if(socket_ec)
                m_module->appendInputValue(socket_ec.message(), "TCP_LOG");
        }
    }
    m_module->addInputValue("SHUTTING_DOWN", "STATUS");
}

boost::system::error_code TCPManagerClient::attemptToConnect()
{
    boost::system::error_code ec; 
    m_socket.reset(new boost::asio::ip::tcp::socket(m_service));
    m_resolver.reset(new boost::asio::ip::tcp::resolver(m_service));
    boost::asio::connect(*m_socket, m_resolver->resolve({m_server_ip, m_server_port}), ec);
    return ec;
}

boost::system::error_code TCPManagerClient::shutdownSocket()
{
    boost::system::error_code ec; 
    if(m_socket)
        m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    return ec;
}

boost::system::error_code TCPManagerClient::closeSocket()
{
    boost::system::error_code ec; 
    if(m_socket)
        m_socket->close(ec);
    return ec;
}

void TCPManagerClient::waitForConnection()
{
    using namespace std::chrono_literals; 

    m_module->addInputValue("DISCONNECTED", "STATUS");

    while(!isOpen())
    {
        boost::system::error_code ec = attemptToConnect();
        if(ec)
            m_module->appendInputValue(ec.message(), "TCP_LOG");

        std::this_thread::sleep_for(5s);
    }
    m_module->addInputValue("CONNECTED", "STATUS");    
}

bool TCPManagerClient::isOpen()
{
    if(m_socket)
        return m_socket->is_open();
    else
        return false;
}

uint64_t TCPManagerClient::write(const void* buffer, const uint64_t buffer_size, boost::system::error_code& ec)
{
    return boost::asio::write(*m_socket, boost::asio::buffer(buffer, buffer_size), ec);
}

uint64_t TCPManagerClient::read(void* buffer, const uint64_t buffer_size, boost::system::error_code& ec)
{
    return m_socket->read_some(boost::asio::buffer(buffer, buffer_size), ec);
}

uint64_t TCPManagerClient::readClient(void* buffer, const uint64_t buffer_size)
{
    while(true)
    {
        boost::system::error_code ec;
        const uint64_t bytes_read = read(buffer, buffer_size, ec);
        if(ec)
        {
            if(ec == boost::asio::error::eof)
            {
                waitForConnection();
            }
            else
            {
                m_module->appendInputValue(ec.message(), "TCP_LOG");
                waitForConnection();
            }
        }
        else
            return bytes_read;
    }
}

uint64_t TCPManagerClient::writeClient(void* buffer, const uint64_t num_bytes)
{
    while(true)
    {
        boost::system::error_code ec;
        const uint64_t bytes_read = write(buffer, num_bytes, ec);
        if(ec)
        {
            if(ec == boost::asio::error::eof)
            {
                waitForConnection();
            }
            else
            {
                m_module->appendInputValue(ec.message(), "TCP_LOG");
                waitForConnection();
            }
        }
        else
            return bytes_read;
    }
}



void updateModules(void* recieve_buffer, const uint64_t buffer_size)
{
    uint64_t current_index = 0;
    const char* buffer = reinterpret_cast<const char*>(recieve_buffer);
    while(current_index < buffer_size)
    {
        const std::string module_header(&buffer[current_index]);
        if(module_header.size() == 0)
            break;
        current_index += module_header.size() + 1;

        const std::string module_value_name(&buffer[current_index]);
        if(module_value_name.size() == 0)
            break;
        current_index += module_value_name.size() + 1;

        const uint32_t value_size = *reinterpret_cast<const uint32_t*>(&buffer[current_index]);
        current_index += sizeof(uint32_t);

        ModuleController::createModule(module_header)->addInputValue(&buffer[current_index], value_size, module_value_name, false);
        current_index += value_size;
    }
}



void TCPManagerClient::runMainLoop()
{
    char read_buffer[2048];
    char ack_string[] = "OK";

    waitForConnection();

    while(true) 
    {
        writeClient(ack_string, sizeof(ack_string)); 
        const uint64_t read_size = readClient(read_buffer, sizeof(read_buffer));
        updateModules(read_buffer, read_size);
    }
}

void TCPManagerClient::run(const std::string& server_ip, const std::string& server_port)
{
    TCPManagerClient client(server_ip, server_port);
    client.runMainLoop();    
}

void TCPManagerClient::start(const std::string& server_ip, const std::string& server_port)
{
    std::thread tcp_client_thread(TCPManagerClient::run, server_ip, server_port);
    tcp_client_thread.detach();
}









