#include "Module.hpp"
#include <atomic>
#include <memory>
#include <cassert>
#include <cstring> 




Module::Module(const std::string& module_header)
    :  m_input_list(), m_output_list(), m_module_header(module_header), m_ready(false)
{}

Module::~Module()
{}

void Module::addInputValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update)
{
    m_input_list.addValue(data_ptr, num_bytes, value_name, always_update); 
}

void Module::addInputValue(const std::string& data, const std::string& value_name, const bool always_update)
{
    m_input_list.addValue(data.c_str(), data.size() + 1, value_name, always_update); 
}

void Module::appendInputValue(const std::string& data, const std::string& value_name, const bool always_update)
{
    m_input_list.appendValue(data.c_str(), data.size() + 1, value_name, always_update); 
}

void Module::addOutputValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update)
{
    m_output_list.addValue(data_ptr, num_bytes, value_name, always_update); 
}

void Module::addOutputValue(const std::string& data, const std::string& value_name, const bool always_update)
{
    m_input_list.addValue(data.c_str(), data.size() + 1, value_name, always_update); 
}

uint32_t Module::copyRawOutputValues(void* buffer, const uint32_t buffer_size, const bool update)
{
    uint32_t current_byte_index = 0;
    std::vector<std::shared_ptr<ModuleValue>> output_list = m_output_list.copyList();
    for(auto output_value : output_list)
    {
        assert(buffer_size > current_byte_index + m_module_header.size() + 1);
        memcpy(reinterpret_cast<uint8_t*>(buffer) + current_byte_index, m_module_header.c_str(), m_module_header.size() + 1);
        current_byte_index += m_module_header.size() + 1;
        current_byte_index += output_value->copyModuleValue(reinterpret_cast<uint8_t*>(buffer) + current_byte_index, buffer_size - current_byte_index, update);
    }
    return current_byte_index;
}

void Module::appendOutputValue(const std::string& data, const std::string& value_name, const bool always_update)
{
    m_input_list.appendValue(data.c_str(), data.size() + 1, value_name, always_update); 
}

std::shared_ptr<ModuleValue> Module::findInputValue(const std::string& value_name)
{
    return m_input_list.findValue(value_name);
}

std::shared_ptr<ModuleValue> Module::findOutputValue(const std::string& value_name)
{
    return m_output_list.findValue(value_name);
}

std::vector<std::shared_ptr<ModuleValue>> Module::copyInputList()
{
    return m_input_list.copyList();
}

std::vector<std::shared_ptr<ModuleValue>> Module::copyOutputList()
{
    return m_output_list.copyList();
}

bool Module::getReadyFlag() const
{
    return m_ready.load(std::memory_order_acquire);
}

void Module::setReadyFlag(const bool flag)
{
    m_ready.store(flag, std::memory_order_release); 
}

const std::string& Module::getModuleHeader() const
{
    return m_module_header;
}



