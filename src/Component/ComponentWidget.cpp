#include "ComponentWidget.h"

#include <QVBoxLayout>

#include "ComponentModel.h"
#include "ComponentView.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
{
   Model* componentModel = new Model(this, central);
   View* componentView = new View(this, central, componentModel);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);

   masterLayout->addWidget(componentView);
}
