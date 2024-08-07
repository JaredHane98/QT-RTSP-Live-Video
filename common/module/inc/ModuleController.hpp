#ifndef MODULECONTROLLER_H
#define MODULECONTROLLER_H
#include "Module.hpp"
#include <boost/atomic.hpp>




class ModuleController
{
private:
    std::vector<std::shared_ptr<Module>> m_modules;
    std::mutex m_mutex;

    static boost::atomic<std::shared_ptr<ModuleController>> m_instance;
    static std::mutex m_instantiation_mutex;
public:
    ~ModuleController();

    ModuleController(ModuleController&&) = delete;
    ModuleController(const ModuleController&) = delete;
private:
    ModuleController();

    static std::shared_ptr<ModuleController> getInstance();
public:
    /**
    @brief Tries to find a module with a given string.
    @param module_name Module name to search.
    @returns Shared_ptr containing module or NULL.
    */
    static std::shared_ptr<Module> findModule(const std::string& module_name);
    /**
    @brief Creates a module with a given string 
    @param module_name Module name to create
    @returns Shared_ptr containing module
    */
    static std::shared_ptr<Module> createModule(const std::string& module_name);
    /**
    @brief Creates a copy of the vector containing all of the modules
    @returns Map containing the strings and pointers to modules
    */
    static std::vector<std::shared_ptr<Module>> getModules();
}; 






#endif // MODULECONTROLLER_H