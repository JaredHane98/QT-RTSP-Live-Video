#include "WindowWorker.hpp"
#include <opencv2/imgproc.hpp>




QTMainWindowWorker::QTMainWindowWorker(QObject* parent, StreamInterface* stream_interface, VideoWidget* widget)
    : QObject(parent), m_current_image(720, 2560, CV_8UC1), m_scaled_image(), m_timer(), m_stream_interface(stream_interface), m_video_widget(widget)
{}

QTMainWindowWorker::~QTMainWindowWorker()
{}

void QTMainWindowWorker::updateBMSTree(const QString& header, std::vector<std::shared_ptr<ModuleValue>>& values)
{
    uint16_t bms_value; // all BMS values uint16_t
    for(auto value : values)
    {
        value->copyData(&bms_value, sizeof(uint16_t));
        const QString value_name = QString::fromStdString(value->getName());
        const QString value_string = QString::number(bms_value);
        emit updateTreeItem(header, value_name, value_string);
    }
}

void QTMainWindowWorker::updateTCPTree(const QString& header, std::vector<std::shared_ptr<ModuleValue>>& values)
{
    char value_string[1000]; // all of tcp values are character strings
    uint32_t value_size = 0;
    for(auto value : values)
    {
        const QString value_name = QString::fromStdString(value->getName());
        if(value_name == "STATUS")  // need to implement log strings here
        {
            value->copyData(value_string, sizeof(value_string));
            emit updateTreeItem(header, value_name, QString(value_string));
        }
    }
}

void QTMainWindowWorker::updateTree()
{
    auto modules = ModuleController::getModules(); 
    for(auto module : modules)
    {
        const QString module_header = QString::fromStdString(module->getModuleHeader());
        auto input_values = module->copyInputList();
        if(module_header == "BMS")
            updateBMSTree(module_header, input_values);
        else if(module_header == "TCP_CLIENT")
            updateTCPTree(module_header, input_values);
    }
}

void QTMainWindowWorker::runThread()
{
    int64_t last_update = m_timer.getTime();
    cv::Mat bordered_image;
    QSize size;
    float frametime_ms;

    while(true)
    {
        if(m_stream_interface->updated())
        {
            frametime_ms = m_stream_interface->copyImage(m_current_image);
            size = m_video_widget->getVideoSize();
            cv::resize(m_current_image, m_scaled_image, cv::Size(size.width(), size.height()));
            
            int32_t border_v = 0, border_h = 0;
            if(m_current_image.cols / m_current_image.rows >= m_scaled_image.cols / m_scaled_image.rows)
                border_v = int32_t((((m_current_image.cols / m_current_image.rows) * m_scaled_image.rows) - m_scaled_image.cols) / 2);
            else
                border_h = int32_t((((m_current_image.rows / m_current_image.cols) *  m_scaled_image.cols) - m_scaled_image.rows) / 2);
            
            if(border_v < 0 || border_h < 0)
                emit updateImage(m_scaled_image);
            else
            {
                cv::copyMakeBorder(m_scaled_image, bordered_image, border_v, border_v, border_h, border_h, cv::BORDER_CONSTANT, 0);
                cv::resize(bordered_image, bordered_image, cv::Size(size.width(), size.height()));
                emit updateImage(bordered_image);
            }
            emit updateTreeItem("RTSP", "Frametime", QString::number(frametime_ms));
        }

        const int64_t time_elasped = m_timer.getTime();
        if(time_elasped - last_update > 50)
        {
            last_update = time_elasped;
            updateTree();
        }
    }
}