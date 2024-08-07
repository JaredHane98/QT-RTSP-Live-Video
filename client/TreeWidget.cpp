#include "TreeWidget.hpp"



TreeWidget::TreeWidget()
    : m_items(), m_layout(new QVBoxLayout()), m_widget(new QTreeWidget())
{
    m_widget->setStyleSheet("color:rgb(193, 195, 199);"
                            "background-color:rgb(37, 37, 37);"
                            "selection-color: rgb(56, 54, 61);"
                            "selection-background-color: grey;"
                            "border: 2px solid rgb(65, 65, 65);");

    m_widget->setColumnCount(2);
    m_widget->setHeaderLabels({{"Module"}, {"Value"}});
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0); 
    m_layout->addWidget(m_widget);
}

TreeWidget::~TreeWidget()
{
    delete m_layout;
    delete m_widget;
}

QTreeWidgetItem* TreeWidget::findChild(QTreeWidgetItem* parent, const QString& name)
{
    for(int32_t i = 0; i < parent->childCount(); i++)
    {
        QTreeWidgetItem* child = parent->child(i);
        if(child->text(0) == name)
            return child;
    }
    return nullptr;
}


void TreeWidget::updateTree(const QString& module_header, const QString& name, const QString& value)
{
    auto found_parent = std::find_if(m_items.begin(), m_items.end(), [&module_header](const QTreeWidgetItem* item){return item->text(0) == module_header;});
    if(found_parent != m_items.end())
    {
        auto child_found = findChild(*found_parent, name);
        if(child_found)
            child_found->setText(1, value);
        else
        {
            QTreeWidgetItem* new_child = new QTreeWidgetItem(*found_parent, QStringList{name, value}); 
            (*found_parent)->addChild(new_child);
        }
    }
    else
    {
        QTreeWidgetItem* new_parent_item = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), QStringList{module_header, ""});
        QTreeWidgetItem* new_child_item = new QTreeWidgetItem(new_parent_item, QStringList{name, value}); 
        new_parent_item->addChild(new_child_item);
        m_items.append(new_parent_item);
        m_widget->insertTopLevelItem(0, new_parent_item);
    }
}

QTreeWidget* TreeWidget::getWidget()
{
    return m_widget;
}

QVBoxLayout* TreeWidget::getLayout()
{
    return m_layout;
}