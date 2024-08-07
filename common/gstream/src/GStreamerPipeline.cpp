#include "GStreamerPipeline.hpp"
#include <iostream> 



bool GStreamPipeline::linkElement(GstElement* element, GstCaps* caps, GstElement* child_element, const std::string& element_name, const std::string& child_element_name)
{
    if(G_UNLIKELY(!GST_IS_ELEMENT(element)))
    {
        std::cout << "Failed to link: " << element_name << " was the pipeline created?\n";
        return false;
    }
    if(G_UNLIKELY(!GST_IS_ELEMENT(child_element)))
    {
        std::cout << "Failed to link " << element_name << " to " << child_element_name << " was the pipeline created?\n";
        return false;
    }
    if(GST_IS_CAPS(caps))
    {
        if(G_UNLIKELY(!gst_element_link_filtered(element, child_element, caps)))
        {
            gchar* tmp_str = gst_caps_to_string(caps);
            std::cout << "Failed to link " << element_name << " to " << child_element_name <<  " with caps: " << tmp_str << '\n';
            gst_caps_unref(caps);
            g_free(tmp_str);
            return false;
        }
        gst_caps_unref(caps);
        return true;
    }
    else if(G_UNLIKELY(!gst_element_link(element, child_element)))
    {
        std::cout << "Failed to link " << element_name << " to " << child_element_name << '\n'; 
        return false;
    }
    return true;
}

GstElement* GStreamPipeline::createElement(const std::string& element, const std::string& element_name, GstElement* pipe)
{   
    GstElement* output;

    if(G_UNLIKELY(pipe == NULL))
        return NULL;
    
    if (G_UNLIKELY (!(output = (gst_element_factory_make (element.c_str(), element_name.c_str()))))) 
        return NULL;
    
    if (G_UNLIKELY (!gst_bin_add (GST_BIN_CAST (pipe), output))) 
    {
        gst_object_unref(G_OBJECT(output));
        return NULL;
    } 
    return output;
}

GStreamPipeline::GStreamPipeline(const std::string& pipeline_name, const bool init_gstream, const bool create_main_loop)
    : m_pipeline_map(), m_pipeline(NULL), m_loop(NULL)
{
    if(init_gstream)
        gst_init(NULL, NULL);
    if(create_main_loop)
        m_loop = g_main_loop_new(NULL, FALSE);
        
    m_pipeline = gst_pipeline_new(pipeline_name.c_str());
}

GStreamPipeline::~GStreamPipeline()
{}

bool GStreamPipeline::addElement(const std::string& element, const std::string& element_name)
{
    GstElement* new_element = createElement(element, element_name, m_pipeline); 
    if(G_UNLIKELY(!GST_IS_ELEMENT(new_element)))
    {
        std::cout << "Failed to add element with name " << element << '\n';
        return false;
    }
    m_pipeline_map[element_name] = {new_element, NULL};
    return true;
}

bool GStreamPipeline::addElement(const std::string& element, const std::string& element_name, const std::string& element_caps)
{
    GstElement* new_element = createElement(element, element_name, m_pipeline); 
    GstCaps* caps = gst_caps_from_string(element_caps.c_str());
    if(G_UNLIKELY(!GST_IS_ELEMENT(new_element)))
    {
        std::cout << "Failed to add element with name " << element << '\n';
        return false;
    }
    if(G_UNLIKELY(!GST_IS_CAPS(caps)))
    {
        std::cout << "Failed to create element caps with string " << element_caps << '\n';
        return false;
    }
    m_pipeline_map[element_name] = {new_element, caps};
    return false;
}

bool GStreamPipeline::linkElementsByName(const std::vector<std::string>& element_names)
{
    for(auto begin = element_names.begin(); begin != element_names.end(); begin++)
    {
        if(begin + 1 == element_names.end())
            break;
            
        auto parent_found = m_pipeline_map.find(*begin);
        auto child_found = m_pipeline_map.find(*(begin + 1));

        if(parent_found == m_pipeline_map.end())
        {
            std::cout << "Failed to link elements. Could not find element by name: " << *begin << '\n';
            return false;
        }
        if(child_found == m_pipeline_map.end())
        {
            std::cout << "Failed to link elements. Could not find child element by name: " << *begin++;
            return false;
        }

        if(!linkElement(std::get<GstElement*>(parent_found->second), std::get<GstCaps*>(parent_found->second), std::get<GstElement*>(child_found->second), parent_found->first, child_found->first))
            return false;
    }
    return true; 
}

bool GStreamPipeline::setElementSignal(const std::string& element_name, const std::string& signal_name, GCallback callback, gpointer user_data)
{
    auto found = m_pipeline_map.find(element_name);
    if(found == m_pipeline_map.end())
        return false;
        
    GstElement* element = std::get<GstElement*>(found->second);
    if(!GST_IS_ELEMENT(element))
        return false;
        
    g_signal_connect(G_OBJECT(element), signal_name.c_str(), G_CALLBACK (callback), user_data);
}




GstElement* GStreamPipeline::getElementByName(const std::string& element_name)
{
    auto found = m_pipeline_map.find(element_name);
    return found != m_pipeline_map.end() ? std::get<GstElement*>(found->second) : NULL;
}

void GStreamPipeline::runMainLoop()
{
    g_main_loop_run(m_loop);
}   

bool GStreamPipeline::setElementState(const std::string& element_name, GstState state)
{
    auto found = m_pipeline_map.find(element_name);
    if(found == m_pipeline_map.end())
        return false;

    return gst_element_set_state(found->second.first, state) != GST_STATE_CHANGE_FAILURE;
}

bool GStreamPipeline::setPipelineState(GstState state)
{
    return gst_element_set_state(m_pipeline, state) != GST_STATE_CHANGE_FAILURE;
}
 
GstElement* GStreamPipeline::attachToBin()
{
    GstElement* bin = gst_bin_new(NULL);
    if(!gst_bin_add(GST_BIN(bin), m_pipeline))
    {
        gst_object_unref(bin);
        return NULL;
    }
    return bin;
}