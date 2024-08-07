#include "MainWindow.hpp"




MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), m_video_widget(QSize(2560, 720)), m_tree_widget(), m_console_widget(), m_grid_layout(new QGridLayout())
{
    m_grid_layout->setSpacing(0);
    m_grid_layout->setContentsMargins(0, 0, 0, 0);
        
    m_grid_layout->addWidget(m_video_widget.getWidget(), 0, 0);
    m_grid_layout->addLayout(m_tree_widget.getLayout(), 0, 1);
    m_grid_layout->addWidget(m_console_widget.getWidget(), 1, 0);

    m_grid_layout->setColumnStretch(0, 8);
    m_grid_layout->setColumnStretch(1, 2);
    m_grid_layout->setRowStretch(0, 8);
    m_grid_layout->setRowStretch(1, 2);

    setStyleSheet("background-color:rgb(37, 37, 37);");
    setLayout(m_grid_layout);
}

MainWindow::~MainWindow()
{
    delete m_grid_layout;
}

void MainWindow::updateImage(const cv::Mat& next_image)
{
    m_video_widget.updateImage(next_image);
}

void MainWindow::updateTree(const QString& module_header, const QString& value_name, const QString& value)
{
    m_tree_widget.updateTree(module_header, value_name, value);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    QRect tree_rect = m_tree_widget.getWidget()->rect();
    QRect console_rect = m_console_widget.getWidget()->rect();
    const QSize resize = QSize(console_rect.width(), tree_rect.height());
    m_video_widget.setImageSize(resize);
}

void MainWindow::startWorkerThread(StreamInterface* interface)
{
    QTMainWindowWorker* m_worker = new QTMainWindowWorker(nullptr, interface, &m_video_widget);
    QThread* thread = new QThread(this);
    m_worker->moveToThread(thread);

    connect(thread, &QThread::started, m_worker, &QTMainWindowWorker::runThread);
    connect(m_worker, SIGNAL(updateImage(const cv::Mat&)), this, SLOT(updateImage(const cv::Mat&)));
    connect(m_worker, SIGNAL(updateTreeItem(const QString&, const QString&, const QString)), this, SLOT(updateTree(const QString&, const QString&, const QString)));
    thread->start();
}

