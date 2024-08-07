#ifndef CONSOLEWIDGET_HPP
#define CONSOLEWIDGET_HPP
#include <QTextEdit>
#include <QString>
#include <QWidget> 




class ConsoleWidget
{
private:
    QTextEdit* m_widget;
public:
    ConsoleWidget();

    ~ConsoleWidget();

    void addLog(const QString& string);

    QTextEdit* getWidget();
}; 








#endif // CONSOLEWIDGET_HPP