#include "ConsoleWidget.hpp"
#include <QScrollBar>
#include <QTextCursor>


ConsoleWidget::ConsoleWidget()
    : m_widget(new QTextEdit())
{
    m_widget->setStyleSheet("color:rgb(193, 195, 199);"
                            "background-color:rgb(37, 37, 37);"
                            "selection-color: rgb(56, 54, 61);"
                            "selection-background-color: grey;"
                            "border: 2px solid rgb(65, 65, 65);");
    m_widget->setReadOnly(true);
    m_widget->setPlainText("Console Logger");
}

ConsoleWidget::~ConsoleWidget()
{
    delete m_widget;
}

void ConsoleWidget::addLog(const QString& string)
{
    QScrollBar* scrollbar = m_widget->verticalScrollBar();
    const int32_t scroll_position = scrollbar->value();
    const int32_t scroll_maximum = scrollbar->maximum();

    m_widget->moveCursor(QTextCursor::End);
    m_widget->insertPlainText(string); 
    m_widget->moveCursor(QTextCursor::End);

    if(scroll_position == scroll_maximum)
        scrollbar->setValue(scrollbar->maximum()); // set to the new maximum
    else
        scrollbar->setValue(scroll_position);      // otherwise set to the previous location
}

QTextEdit* ConsoleWidget::getWidget()
{
    return m_widget;
}
