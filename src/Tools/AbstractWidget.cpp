#include "AbstractWidget.h"

#include <QVBoxLayout>

Abstract::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
   , toolBar(nullptr)
   , masterLayout(nullptr)
{
   toolBar = new QToolBar(this);
   toolBar->setMovable(false);

   masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(toolBar);
}

void Abstract::Widget::setPayload(QWidget* widget)
{
   masterLayout->addWidget(widget);
}
