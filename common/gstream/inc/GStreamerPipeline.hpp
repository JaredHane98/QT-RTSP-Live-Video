#ifndef GSTREAMER_PIPELINE_HPP
#define GSTREAMER_PIPELINE_HPP

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/rtsp/gstrtsptransport.h>
#include <map>
#include <string>
#include <vector>
#include <utility>

class GStreamPipeline
{
private:
    typedef std::pair<GstElement*, GstCaps*> gstream_pair;
    typedef std::map<std::string, gstream_pair> gstream_map;

    gstream_map m_pipeline_map;
    GstElement* m_pipeline;
    GMainLoop*  m_loop;
    /**
    @brief creates a gstreame element
    @param element Gstelement to link
    @param caps caps filter can be NULL
    @param child_element child to link
    @param element_name element name
    @param child_element_name child_element_name
    @return true on success false on failure
    */
    bool linkElement(GstElement* element, GstCaps* caps, GstElement* child_element, const std::string& element_name, const std::string& child_element_name);
    /**
    @brief creates a gstream element 
    @param element gstElement plugin name
    @param element_name element name
    @param pipe to attach
    @returns contructed element or NULL on failure
    */
    GstElement* createElement(const std::string& element, const std::string& element_name, GstElement* pipe);
public:
    /**
    @brief constructs a pipeline with the provided arguments
    @param pipeline_name name of the pipeline to construct
    @param init_gstream initialize gstreamer in constructor?
    @param create_main_loop create a GMainLoop in constructor? 
    */
    GStreamPipeline(const std::string& pipeline_name, const bool init_gstream = true, const bool create_main_loop = true);
    /**
    @brief does nothing
    */
    ~GStreamPipeline();

    GStreamPipeline(GStreamPipeline&&) = delete;
    GStreamPipeline(const GStreamPipeline&) = delete;
    /**
    @brief adds an element to the pipeline
    @param element GstElement plugin name
    @param element_name element_name
    @returns true on success false on failure
    */
    bool addElement(const std::string& element, const std::string& element_name);
    /**
    @brief adds a element to the pipeline
    @param element GstElement plugin name
    @param element_name element_name
    @param element_caps element_caps string
    */
    bool addElement(const std::string& element, const std::string& element_name, const std::string& element_caps);
    /** 
    @brief Links the elements by list of name
    @param element_name list of nanes
    @return if successfully linked 
    */
    bool linkElementsByName(const std::vector<std::string>& element_names);
    /**
    @brief Sets the element signal to supplied callback
    @param element element name
    @param signal_name name of the signal to attach the callback to
    @param callback function callback
    @param user_data user data. can be NULL
    @returns true if the signal was set correctly. Doesn't guaranteed the Element was actually set because the API doesn't have any way to check  
    */
    bool setElementSignal(const std::string& element_name, const std::string& signal_name, GCallback callback, gpointer user_data);
    /**
    @brief Sets the element property to the provided value
    @param element elements name
    @param propery_name the properties name the value will be set to
    @param propery_value the value to set
    @return true if the elements property was set correctly. Doesn't guaranteed the property was actually set because the API doesn't have any way to check  
    */
    template<typename T>
    bool setElementProperty(const std::string& element_name, const std::string& property_name, T property_value)
    {
        auto found = m_pipeline_map.find(element_name);
        if(found == m_pipeline_map.end())
            return false;

        GstElement* element = std::get<GstElement*>(found->second);
        if(!GST_IS_ELEMENT(element))
            return false;
        
        g_object_set(G_OBJECT(element), property_name.c_str(), property_value, NULL);
    }
    /**
    @brief Returns the GstElement by name
    @param element_name the GstElement name
    @returns See above
    */
    GstElement* getElementByName(const std::string& element_name);
    /**
    @brief Runs the main loop
    */
    void runMainLoop();  
    /**
    @brief sets the element state
    @param element_name
    @param element_state 
    @returns if the state was set
    */
    bool setElementState(const std::string& element_name, GstState state);
    /**
    @brief sets the pipeline state
    @param state pipeline state to set
    @returns if the state was set
    */
    bool setPipelineState(GstState state);
    /**
    @brief attaches the pipeline to a bin
    @returns attached bin, or NULL if failed
    */
    GstElement* attachToBin();
};









#endif // GSTREAMER_PIPELINE_HPP