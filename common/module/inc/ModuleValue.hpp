#ifndef MODULEVALUE_H
#define MODULEVALUE_H

#include <atomic> 
#include <mutex>
#include <string>
#include <memory>
#include <stdint.h>


class Module;

class ModuleValue
{
private:
    std::mutex m_mutex;                        /* lock                           */
    std::string m_value_name;                  /* module value name              */
    uint8_t *m_data_ptr;                       /* data                           */
    uint32_t m_num_bytes;                      /* data_ptr bytes or message size */
    std::atomic<bool> m_updated;               /* has been updated?              */
    const bool m_always_update;                /* always update module value     */
public:
    /**
    @brief Constructs a new module value
    @param data_ptr Module data
    @param num_bytes Module data size in bytes
    @param value_name Module value name 
    @param alway_update Whether this value should always be percieved updated
    */
    ModuleValue(const void* data_ptr, const uint32_t num_bytes, const std::string value_name, const bool always_update);
    
    ~ModuleValue();
    
    ModuleValue() = delete;
    ModuleValue(ModuleValue&&) = delete;
    ModuleValue(const ModuleValue&) = delete;
    /**
    @brief Copies the module data
    @param buffer Buffer to copy to
    @param buffer_size Buffer size in bytes
    @param update whether to mark this as not updated. Default=True. Overridden by always_update
    @returns Number of bytes copied 
    */
    uint32_t copyData(void* buffer, const uint32_t buffer_size, const bool update = true);
    /**
    @brief copies the entire ModuleValue. value_name, num_bytes, data_ptr
    @param buffer buffer to place ModuleValue
    @param buffer_size size of the buffer
    @param updated whether to mark this as not updated. Default=True. Overridden by always_update
    @return number of bytes copied
    */
    uint32_t copyModuleValue(void* buffer, const uint32_t buffer_size, const bool update = true);
    /**
    @brief Updates the module values data
    @param buffer Buffer to write
    @param data_size data size in bytes
    @param update whether to make as updated. Default = True
    */
    void updateData(const void* buffer, const uint32_t data_size, const bool update = true);
    /**
    @brief Appends the data 
    @param buffer Buffer to copy to
    @param data_size data size in bytes
    @param update whether to make as updated. Default = True
    */
    void appendData(const void* buffer, const uint32_t data_size, const bool update = true);
    /**
    @brief Updates the module value
    @param value ModuleValue
    */
    void updateData(const std::shared_ptr<ModuleValue>& value);
    /**
    @brief Retuns module value name
    @returns see above
    */
    const std::string& getName() const;
    /**
    @brief Retuns if value has been updated
    @returns see above
    */
    bool updated() const;
    /**
    @brief Returns module value size
    @returns see above
    */
    uint32_t getSize();
    /**
    @brief Returns if value is nullptr
    @returns see above
    */
    bool isNull() const;
    /**
    @brief Sets the update flag. Unless always_update is true
    @param flag value to set to update flag
    */
    void setUpdate(const bool flag);
};




#endif // MODULEVALUE_H





