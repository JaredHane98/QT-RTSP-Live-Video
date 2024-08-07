#ifndef WINDOWWORKER_HPP
#define WINDOWWORKER_HPP
#include <QObject>
#include <QImage>
#include <QString>
#include <opencv2/core/mat.hpp>
#include "ModuleController.hpp"
#include "Timer.hpp"
#include "VideoWidget.hpp"
#include "StreamInterface.hpp"






class QTMainWindowWorker : public QObject
{
    Q_OBJECT
private:
    cv::Mat m_current_image;
    cv::Mat m_scaled_image;
    Timer<std::milli> m_timer;
    StreamInterface* m_stream_interface;
    VideoWidget* m_video_widget;
public:
    QTMainWindowWorker(QObject* parent, StreamInterface* stream_interface, VideoWidget* mainwindow);

    ~QTMainWindowWorker();

    void runThread();
private:
    void updateBMSTree(const QString& bms_header, std::vector<std::shared_ptr<ModuleValue>>& input_values);

    void updateTCPTree(const QString& tcp_header, std::vector<std::shared_ptr<ModuleValue>>& input_values);

    void updateTree();
signals:
    void updateImage(const cv::Mat& image);
    void updateTreeItem(const QString& module_header, const QString& value_name, const QString& value);
};



#endif // WINDOWWORKER_HPP