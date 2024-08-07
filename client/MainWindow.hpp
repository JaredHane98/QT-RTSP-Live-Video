#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <QMainWindow>
#include <QGridLayout>
#include <QThread>
#include <QVideoSurfaceFormat>
#include <QVideoWidget>
#include <QAbstractVideoSurface>
#include <iostream>



#include "TreeWidget.hpp"
#include "ConsoleWidget.hpp"
#include "Timer.hpp"
#include "WindowWorker.hpp"





class MainWindow : public QWidget
{
    Q_OBJECT
private:
    VideoWidget m_video_widget;
    TreeWidget m_tree_widget;
    ConsoleWidget m_console_widget;
    QGridLayout* m_grid_layout;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
public slots:
    void updateImage(const cv::Mat& next_image);
    void updateTree(const QString& module_header, const QString& value_name, const QString& value);
protected:
    void resizeEvent(QResizeEvent* event);
public:
    void startWorkerThread(StreamInterface* interface);
};




#endif // WINDOW_HPP