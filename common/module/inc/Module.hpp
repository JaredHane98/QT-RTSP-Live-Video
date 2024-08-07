#ifndef MODULE_H 
#define MODULE_H
#include "ModuleValueList.hpp"


class Module
{
private:
    ModuleValueList m_input_list;       /* List of input values to the module*/
    ModuleValueList m_output_list;      /* List of output values to the module */
    const std::string m_module_header;  /* The identifying string header of the module */
    std::atomic<bool> m_ready;          /* Atomic flag sometimes used */
public:
    Module(const std::string& module_header);

    ~Module();

    Module() = delete;
    Module(const Module&) = delete;
    Module(Module&&) = delete;
    /**
    @brief Adds module value to the Input list
    @param data_ptr data_ptr of the module_value
    @param num_bytes size of data_ptr in bytes
    @param value_name value name of the moduleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void addInputValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update = false);
    /**
    @brief Overload for strings
    @param data data in form of a string
    @param value_name value name of the moduleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void addInputValue(const std::string& data, const std::string& value_name, const bool always_update = false);
    /**
    @brief Checks if value_name exists and appends the string to the message rather than overwriting it
    @param data data in form of a string
    @param value_name value_name fo the ModuleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void appendInputValue(const std::string& data, const std::string& value_name, const bool always_update = false);
    /**
    @brief Adds module value to the output list
    @param data_ptr data_ptr of the module_value
    @param num_bytes size of data_ptr in bytes
    @param value_name value name of the moduleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void addOutputValue(const void* data_ptr, const uint32_t num_bytes, const std::string& value_name, const bool always_update = false);
    /**
    @brief Overload for strings
    @param data data in form of a string
    @param value_name value name of the moduleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void addOutputValue(const std::string& data, const std::string& value_name, const bool always_update = false);
    /**
    @brief Copies the output values and places them into buffer. Format Module_Header, Value_Name, Value_Size, Value_Data
    @param buffer buffer to copy data into
    @param num_bytes size of the buffer in bytes
    @return number of bytes copied
    */
    uint32_t copyRawOutputValues(void* buffer, const uint32_t buffer_size, const bool update = true);
    /**
    @brief Checks if value_name exists and appends the string to the message rather than overwriting it
    @param data data in form of a string
    @param value_name value_name fo the ModuleValue
    @param always_update whether this modulevalue should always be considered updated
    */
    void appendOutputValue(const std::string& data, const std::string& value_name, const bool always_update = false);
    /**
    @brief Tries to find an input ModuleValue with provided name
    @param value_name value_name to search for
    @returns found module value or nullptr
    */
    std::shared_ptr<ModuleValue> findInputValue(const std::string& value_name);
    /**
    @brief Tries to find a output ModuleValue with provided name
    @param value_name value_name to search for
    @returns found module value or nullptr
    */
    std::shared_ptr<ModuleValue> findOutputValue(const std::string& value_name);
    /**
    @brief Copies the input list. 
    @returns copy of the input list
    */
    std::vector<std::shared_ptr<ModuleValue>> copyInputList();
    /**
    @brief Copies the input list. 
    @returns copy of the input list
    */
    std::vector<std::shared_ptr<ModuleValue>> copyOutputList();
    /**
    @brief returns ready flag
    @return ready flag
    */
    bool getReadyFlag() const;
    /**
    @brief sets ready flag
    @param flag ready flag
    */
    void setReadyFlag(const bool flag);
    /**
    @brief returns module header string
    @return string module header
    */
    const std::string& getModuleHeader() const;
}; 














#endif // MODULE_H