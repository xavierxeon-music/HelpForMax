#include "SelectWidget.h"

#include <QVBoxLayout>

#include "SelectModel.h"
#include "SelectView.h"

Select::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
{
   Model* selectModel = new Model(this, central);
   View* selectView = new View(this, central, selectModel);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);

   masterLayout->addWidget(selectView);
}
