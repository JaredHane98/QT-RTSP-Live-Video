#ifndef TREEWIDGET_HPP
#define TREEWIDGET_HPP
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QList>



// class ElideLeftDelegate: public QStyledItemDelegate
// {
// public:
//     using QStyledItemDelegate::QStyledItemDelegate;
// protected:
//     void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const{
//         QStyledItemDelegate::initStyleOption(option, index);
//         option->textElideMode = Qt::ElideLeft;
//     }
// };



class TreeWidget
{
private:
    QList<QTreeWidgetItem*> m_items;
    QVBoxLayout* m_layout;
    QTreeWidget* m_widget;
public:
    TreeWidget();

    ~TreeWidget();

    TreeWidget(const TreeWidget&) = delete;
    TreeWidget(TreeWidget&&) = delete;

    void updateTree(const QString& module_header, const QString& name, const QString& value);

    QTreeWidget* getWidget();

    QVBoxLayout* getLayout();
private:
    QTreeWidgetItem* findChild(QTreeWidgetItem* parent, const QString& name);
};




#endif // TREEWIDGET_HPP