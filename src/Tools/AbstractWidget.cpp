#include "AbstractWidget.h"

#include <QLabel>
#include <QVBoxLayout>

Abstract::Widget::Widget(QWidget* parent, Central* central, const QString tilte)
   : QWidget(parent)
   , central(central)
   , toolBar(nullptr)
   , masterLayout(nullptr)
{
   toolBar = new QToolBar(this);
   toolBar->setMovable(false);

   QLabel* nameLabel = new QLabel(this);
   QFont font = nameLabel->font();
   font.setBold(true);
   nameLabel->setFont(font);
   nameLabel->setText(tilte.toUpper());

   masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(nameLabel);
   masterLayout->addWidget(toolBar);
}

void Abstract::Widget::setPayload(QWidget* widget)
{
   masterLayout->addWidget(widget);
}
