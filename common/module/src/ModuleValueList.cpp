#include "ModuleValueList.hpp"
#include <algorithm>

ModuleValueList::ModuleValueList()
    : m_mutex(), m_values()
{}

ModuleValueList::~ModuleValueList()
{}

void ModuleValueList::addValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    
    auto found_value = std::find_if(m_values.begin(), m_values.end(), 
                                    [&value_name](std::shared_ptr<ModuleValue> value){return value->getName() == value_name;});
    
    if(found_value != m_values.end())
        (*found_value)->updateData(data_ptr, num_bytes);
    else
        m_values.push_back(std::make_shared<ModuleValue>(data_ptr, num_bytes, value_name, always_update));
}

void ModuleValueList::appendValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    auto found_value = std::find_if(m_values.begin(), m_values.end(), 
                                    [&value_name](std::shared_ptr<ModuleValue> value){return value->getName() == value_name;});

    if(found_value != m_values.end())
        (*found_value)->updateData(data_ptr, num_bytes);
    else
        m_values.push_back(std::make_shared<ModuleValue>(data_ptr, num_bytes, value_name, always_update));
                                    
}

std::shared_ptr<ModuleValue> ModuleValueList::findValue(const std::string& value_name)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    auto found_value = std::find_if(m_values.begin(), m_values.end(), 
                                    [&value_name](std::shared_ptr<ModuleValue> value){return value->getName() == value_name;});
    
    if(found_value != m_values.end())
        return *found_value;
    else
        return nullptr;
}

std::vector<std::shared_ptr<ModuleValue>> ModuleValueList::copyList()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_values;
}