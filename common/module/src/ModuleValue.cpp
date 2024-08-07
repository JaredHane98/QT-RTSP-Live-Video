#include "ModuleValue.hpp"
#include <atomic>
#include <cassert>
#include <cstring>
#include <stdlib.h>


ModuleValue::ModuleValue(const void* data_ptr, const uint32_t num_bytes, const std::string value_name, const bool always_update)
    : m_mutex(), m_value_name(value_name), m_data_ptr(NULL), m_num_bytes(num_bytes), m_updated(true), m_always_update(always_update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if(data_ptr != NULL && num_bytes != 0)
    {
        m_data_ptr = (uint8_t*)malloc(m_num_bytes);
        assert(m_data_ptr != NULL);
        memcpy(m_data_ptr, data_ptr, m_num_bytes);
    }
}

ModuleValue::~ModuleValue()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if(m_data_ptr != NULL)
        free(m_data_ptr);
}

uint32_t ModuleValue::copyData(void* buffer, const uint32_t buffer_size, const bool update)  
{
    std::lock_guard<std::mutex> guard(m_mutex);
    assert(buffer_size >= m_num_bytes && buffer != NULL);

    if(m_data_ptr != NULL)
        memcpy(buffer, m_data_ptr, m_num_bytes);

    if(!m_always_update && update)
        m_updated.store(false, std::memory_order_release);

    return m_num_bytes;
}

uint32_t ModuleValue::copyModuleValue(void* buffer, const uint32_t buffer_size, const bool update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    const uint32_t module_size = m_value_name.size() + 1 + m_num_bytes + sizeof(uint32_t);
    assert(buffer_size >= module_size);

    memcpy(buffer, m_value_name.c_str(), m_value_name.size() + 1);
    memcpy(reinterpret_cast<uint8_t*>(buffer) + m_value_name.size() + 1, &m_num_bytes, sizeof(uint32_t));
    memcpy(reinterpret_cast<uint8_t*>(buffer) + m_value_name.size() + 1 + sizeof(uint32_t), m_data_ptr, m_num_bytes);

    if(!m_always_update && update)
        m_updated.store(false, std::memory_order_release);

    return module_size;
}

void ModuleValue::updateData(const std::shared_ptr<ModuleValue>& value)
{
    std::lock_guard<std::mutex> guard(value->m_mutex);
    updateData(value->m_data_ptr, value->m_num_bytes);
}



void ModuleValue::updateData(const void* buffer, const uint32_t data_size, const bool update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    assert(buffer != nullptr && data_size > 0); 

    if(data_size == m_num_bytes)
        memcpy(m_data_ptr, buffer, m_num_bytes); // I could have a second parameter called actual_size but this super simple. 
    else                                         // Just allocate whenever the sizes are not the same.
    {
        free(m_data_ptr);
        m_num_bytes = data_size;
        m_data_ptr = (uint8_t*)malloc(m_num_bytes); 
        assert(m_data_ptr != nullptr);
        memcpy(m_data_ptr, buffer, m_num_bytes); 
    }

    if(update)
        m_updated.store(true, std::memory_order_release);
}

void ModuleValue::appendData(const void* buffer, const uint32_t data_size, const bool update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    assert(buffer != nullptr && data_size > 0); 

    const uint32_t new_size = data_size + m_num_bytes; 
    uint8_t* new_buffer = (uint8_t*)malloc(new_size);
    assert(new_buffer != nullptr);

    memcpy(new_buffer, m_data_ptr, m_num_bytes);
    memcpy(new_buffer + m_num_bytes, buffer, data_size);

    free(m_data_ptr);
    m_data_ptr = new_buffer;
    m_num_bytes = new_size;

    if(update)
        m_updated.store(true, std::memory_order_release);
}


const std::string& ModuleValue::getName() const
{
    return m_value_name;
}

bool ModuleValue::updated() const 
{
    return m_updated.load(std::memory_order_acquire); 
}

uint32_t ModuleValue::getSize()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_num_bytes;
}

bool ModuleValue::isNull() const 
{
    return m_data_ptr == nullptr;
}

void ModuleValue::setUpdate(const bool flag)
{
    m_updated.store(flag, std::memory_order_release);
}






