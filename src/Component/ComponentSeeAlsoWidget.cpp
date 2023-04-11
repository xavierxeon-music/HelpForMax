#include "ComponentSeeAlsoWidget.h"

Component::SeeAlsoWidget::SeeAlsoWidget(QWidget* parent)
   : QTreeView(parent)
   , model(nullptr)
{
   model = new QStandardItemModel(this);
   setModel(model);
}

