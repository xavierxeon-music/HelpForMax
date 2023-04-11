#include "ComponentWidget.h"

#include "Components/ComponentModel.h"
#include "Components/ComponentView.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
{
   ComponentsModel* componentsModel = new ComponentsModel(this);
   ComponentsView* componentsView = new ComponentsView(this, componentsModel);
}

