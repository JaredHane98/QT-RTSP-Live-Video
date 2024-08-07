#ifndef VIDEOWIDGET_HPP
#define VIDEOWIDGET_HPP
#include <QSize>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QVideoWidget>
#include <QImage>
#include <opencv2/core/mat.hpp>
#include <mutex>





class VideoWidget
{
private:
    QImage m_image;
    QVideoWidget* m_widget;
    std::mutex m_mutex;
public:
    VideoWidget(const QSize& image_size);

    ~VideoWidget();

    void setImageSize(const QSize& image_size);

    void updateImage(const cv::Mat &next_image);

    QSize getVideoSize();

    QVideoWidget* getWidget();
}; 




#endif // VIDEOWIDGET_HPP