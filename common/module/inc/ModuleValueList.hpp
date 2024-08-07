#ifndef MODULEVALUELIST_H
#define MODULEVALUELIST_H
#include "ModuleValue.hpp"
#include <vector>



class ModuleValueList
{
private:
    std::mutex m_mutex;
    std::vector<std::shared_ptr<ModuleValue>> m_values;
public:
    ModuleValueList();

    ~ModuleValueList();

    ModuleValueList(const ModuleValueList&) = delete; 
    ModuleValueList(ModuleValueList&&) = delete;
    /**
    @brief Adds ModuleValue to the list. Updates if the value exists already
    @param data_ptr values data
    @param num_bytes size of the data in bytes
    @param value_name identifying name of the value
    @param always_update whether value should always be considered updated
    */
    void addValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update);
    /**
    @brief Adds ModuleValue to the list. Appends the data if the value exists already
    @param data_ptr values data
    @param num_bytes size of the data in bytes
    @param value_name identifying name of the value
    @param always_update whether value should always be considered updated
    */
    void appendValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update);
    /**
    @brief Tries to find the module value with the name provided
    @param value_name to search for
    @return found ModuleValue or NULL
    */
    std::shared_ptr<ModuleValue> findValue(const std::string& value_name);
    /**
    @brief Makes a copy of the list of values. Adding to the list will not be present inside of the class
    @return A copy of the modulelist
    */
    std::vector<std::shared_ptr<ModuleValue>> copyList();
}; 











#endif // MODULEVALUELIST_H 