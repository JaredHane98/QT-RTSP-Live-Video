#include "StreamInterface.hpp"
#include <atomic>


StreamInterface::StreamInterface()
        : m_mutex(), m_image(720, 2560, CV_8UC1), m_timer(), m_missed_frames(0), m_updated(false)
{}

StreamInterface::~StreamInterface()
{}

bool StreamInterface::updated()
{
    return m_updated.load(std::memory_order_acquire);
}
void StreamInterface::updateImage(GstBuffer* buffer)
{
    GstMapInfo buffer_info;
    gst_buffer_map(buffer, &buffer_info, GST_MAP_READ);

    assert(buffer_info.size == m_image.total() * m_image.elemSize());

    std::lock_guard<std::mutex> guard(m_mutex);

    memcpy(m_image.data, buffer_info.data, buffer_info.size);

    if(m_updated.load(std::memory_order_acquire))
        m_missed_frames++;

    m_updated.store(true, std::memory_order_release);
    gst_buffer_unmap(buffer, &buffer_info); 
}

float StreamInterface::copyImage(cv::Mat& dest)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    assert(dest.total() * dest.elemSize() == m_image.total() * m_image.elemSize());

    memcpy(dest.data, m_image.data, m_image.total() * m_image.elemSize());
    m_updated.store(false, std::memory_order_release);
    m_timer.getTimeElapsed();
}

int32_t StreamInterface::getMissedFrames()
{
    return m_missed_frames.exchange(0);
}

