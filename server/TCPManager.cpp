#include "TCPManager.hpp"
#include "ModuleController.hpp"
#include <iostream> 
#include <chrono>
#include <thread> 





TCPManagerServer::TCPManagerServer(const int16_t port)
    : m_service(), m_acceptor(), m_socket(), m_port(port)
{}

TCPManagerServer::~TCPManagerServer()
{
    if(isOpen())
    {
        boost::system::error_code shutdown = shutdownSocket();
        if(shutdown)
            std::cout << "An error occurred while shutting down " << shutdown.message() << '\n'; 
        else
        {
            boost::system::error_code socket_ec = closeSocket();
            if(socket_ec)
                std::cout << "An error ocurred while closing the socket " << shutdown.message() << '\n';
        }
    }
}

bool TCPManagerServer::isOpen()
{
    if(m_socket)
        return m_socket->is_open();
    else
        return false;
}

boost::system::error_code TCPManagerServer::shutdownSocket()
{
    boost::system::error_code ec;
    if(m_socket)
        m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec); // why does this have return boost::system::error_code when we pass it by reference? 
    return ec;
}

boost::system::error_code TCPManagerServer::closeSocket()
{
    boost::system::error_code ec;
    if(m_socket)
        m_socket->close(ec);
    return ec;
}

boost::system::error_code TCPManagerServer::acceptConnection()
{
    boost::system::error_code ec;
    m_acceptor.reset(new boost::asio::ip::tcp::acceptor(m_service, tcp::endpoint(tcp::v4(), m_port)));
    m_socket.reset(new boost::asio::ip::tcp::socket(m_service));
    m_acceptor->accept(*m_socket, ec);
    return ec;
}

uint64_t TCPManagerServer::read(void* buffer, const uint64_t buffer_size, boost::system::error_code& ec)
{
    return m_socket->read_some(boost::asio::buffer(buffer, buffer_size), ec);   
}

uint64_t TCPManagerServer::write(const void* buffer, const uint64_t buffer_size, boost::system::error_code& ec)
{
    return boost::asio::write(*m_socket, boost::asio::buffer(buffer, buffer_size), ec);
}

void TCPManagerServer::waitForConnection()
{
    using namespace std::chrono_literals; 
    while(!isOpen())
    {
        boost::system::error_code ec = acceptConnection(); // fix me
        if(ec)
        {
            std::cout << "An error occurred while waiting for a connection " << ec.message() << '\n';
        }
        std::this_thread::sleep_for(5s);
    }
}

uint64_t TCPManagerServer::writeServer(const void* buffer, const uint64_t buffer_size)
{
    while(true)
    {
        boost::system::error_code ec;
        const uint64_t bytes_written = write(buffer, buffer_size, ec);
        if(ec)
        {
            if(ec == boost::asio::error::eof)
            {
                waitForConnection();
            }
            else
            {
                assert(!ec);

                std::cout << "An unknown error while writing " << ec.message() << '\n';
                waitForConnection();
            }
        }
        else
            return bytes_written;
    }
}

uint64_t TCPManagerServer::readServer(void* buf, const uint64_t num_bytes)
{
    while(true)
    {
        boost::system::error_code ec;
        const uint64_t bytes_read = read(buf, num_bytes, ec);

        if(ec)
        {
            if(ec == boost::asio::error::eof)
            {
                waitForConnection();
            }
            else
            {
                std::cout << "An unknown error occurred " << ec.message() << '\n';
                waitForConnection();
            }
        }
        else
            return bytes_read;
    }
}



void TCPManagerServer::readAck()
{
    const char accepted_ack_message[] = "OK";
    char ack_message_buffer[3];
    while(true)
    {
        const uint64_t bytes_read = readServer(ack_message_buffer, sizeof(ack_message_buffer));
        if(ack_message_buffer[2] == '\0')
            if(strcmp(accepted_ack_message, ack_message_buffer) == 0)
                break;
    }
    
}

void TCPManagerServer::runMainLoop()
{
    uint8_t tcp_buffer[2048];

    waitForConnection();

    while(true)
    {
        readAck();

        auto modules = ModuleController::getModules();
        uint32_t message_size = 0;
        for(auto module : modules)
            message_size += module->copyRawOutputValues(tcp_buffer, sizeof(tcp_buffer) - message_size);
        
        writeServer(tcp_buffer, message_size);
    }
}

void TCPManagerServer::run(const uint16_t port)
{
    TCPManagerServer tcp_server(port);
    tcp_server.runMainLoop();
}

void TCPManagerServer::start(const uint16_t port)
{
    std::thread tcp_server_thread(TCPManagerServer::run, port);
    tcp_server_thread.detach();
}
