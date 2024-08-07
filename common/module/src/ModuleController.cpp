#include "ModuleController.hpp"
#include <algorithm>




boost::atomic<std::shared_ptr<ModuleController>> ModuleController::m_instance(nullptr);
std::mutex ModuleController::m_instantiation_mutex;




ModuleController::ModuleController()
{}

ModuleController::~ModuleController()
{}

std::shared_ptr<ModuleController> ModuleController::getInstance()
{
    std::shared_ptr<ModuleController> tmp = m_instance.load(boost::memory_order_consume);
    if(!tmp)
    {
        std::lock_guard<std::mutex> guard(m_instantiation_mutex);
        tmp = m_instance.load(boost::memory_order_consume);
        if(!tmp)
        {
            struct make_shared_enabler : public ModuleController {};
            tmp = std::make_shared<make_shared_enabler>();
            assert(tmp != nullptr);
            m_instance.store(tmp, boost::memory_order_release);
        }
    }
    return tmp;
}

std::shared_ptr<Module> ModuleController::findModule(const std::string& module_header)
{
    std::shared_ptr<ModuleController> controller = ModuleController::getInstance();
    std::lock_guard<std::mutex> guard(controller->m_mutex);
    auto found = std::find_if(controller->m_modules.begin(), controller->m_modules.end(), 
                        [&module_header](const std::shared_ptr<Module> module) { return module_header == module->getModuleHeader();});
    return found == controller->m_modules.end() ? nullptr : *found;
}

std::shared_ptr<Module> ModuleController::createModule(const std::string& module_header)
{
    std::shared_ptr<Module> found_module = ModuleController::findModule(module_header);
    if(!found_module)
    {
        std::shared_ptr<ModuleController> controller = ModuleController::getInstance();
        std::lock_guard<std::mutex> guard(controller->m_mutex);
        found_module = std::make_shared<Module>(module_header);
        controller->m_modules.push_back(found_module);
    }
    return found_module;
}

std::vector<std::shared_ptr<Module>> ModuleController::getModules()
{
    std::shared_ptr<ModuleController> controller = ModuleController::getInstance();
    std::lock_guard<std::mutex> guard(controller->m_mutex);
    return controller->m_modules;
}
