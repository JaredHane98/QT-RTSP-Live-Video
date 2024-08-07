#ifndef STREAM_INTERFACE_HPP
#define STREAM_INTERFACE_HPP
#include <mutex>
#include <atomic>
#include <QImage> 
#include <opencv2/core/mat.hpp>
#include <gst/gst.h>
#include <cassert>
#include "Timer.hpp"


class StreamInterface
{
private:
    std::mutex m_mutex; 
    cv::Mat m_image;
    Timer<std::milli> m_timer;
    std::atomic<int32_t> m_missed_frames; 
    std::atomic<bool> m_updated;
public:
    StreamInterface();
    ~StreamInterface();

    StreamInterface(const StreamInterface&&) = delete; 
    StreamInterface(StreamInterface&) = delete; 

    bool updated(); 
    
    void updateImage(GstBuffer* buffer);

    float copyImage(cv::Mat& dest);

    int32_t getMissedFrames();
};


















#endif // STREAM_INTERFACE_HPP