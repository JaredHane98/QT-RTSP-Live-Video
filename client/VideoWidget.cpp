#include "VideoWidget.hpp"
#include <opencv2/imgproc.hpp>
#include <QAbstractVideoSurface>
#include <iostream>

VideoWidget::VideoWidget(const QSize& image_size)
        : m_image(image_size, QImage::Format_Grayscale8), m_widget(new QVideoWidget()), m_mutex()
{
    m_widget->videoSurface()->start(QVideoSurfaceFormat(image_size, QVideoFrame::Format_Y8));
    m_widget->videoSurface()->present(m_image);
} 

VideoWidget::~VideoWidget()
{
    delete m_widget;
}

void VideoWidget::setImageSize(const QSize& image_size)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if(image_size != m_image.size())
    {
        cv::Mat scaled_image;
        cv::Mat prev_image(m_image.height(), m_image.width(), CV_8UC1, m_image.bits(), m_image.bytesPerLine());

        cv::resize(prev_image, scaled_image, cv::Size(image_size.width(), image_size.height()));

        m_image = QImage(scaled_image.data, scaled_image.cols, scaled_image.rows, scaled_image.step, QImage::Format_Grayscale8).copy();

        m_widget->videoSurface()->start(QVideoSurfaceFormat(image_size, QVideoFrame::Format_Y8));
        m_widget->videoSurface()->present(m_image);
    }
}

void VideoWidget::updateImage(const cv::Mat &next_image)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    const QSize current_size = m_image.size();
    if(next_image.cols != current_size.width() || next_image.rows != current_size.height()) // unless the size is rapidly changing this won't be called
    {
        cv::Mat scaled_image;
        cv::resize(next_image, scaled_image, cv::Size(current_size.width(), current_size.height()));

        if(next_image.total() * next_image.elemSize() != m_image.sizeInBytes())             // For unknown reasons, QImage can randomly pad the image. So we have to check if the size is the same
            m_image = QImage(scaled_image.data, scaled_image.cols, scaled_image.rows, scaled_image.step, QImage::Format_Grayscale8).copy();
        else
            memcpy(m_image.bits(), scaled_image.data, scaled_image.total() * scaled_image.elemSize());
    }
    else
    {
        if(next_image.total() * next_image.elemSize() != m_image.sizeInBytes())             
            m_image = QImage(next_image.data, next_image.cols, next_image.rows, next_image.step, QImage::Format_Grayscale8);
        else
            memcpy(m_image.bits(), next_image.data, next_image.total() * next_image.elemSize());
    }
    m_widget->videoSurface()->present(m_image);
}

QSize VideoWidget::getVideoSize() 
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_image.size();
}

QVideoWidget* VideoWidget::getWidget()
{
    return m_widget;
}

