#include "PageBlank.h"

Page::Blank::Blank(QWidget* parent, Central* central, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
{
   setupUi(this);

   const QIcon icon(":/HelpForMax.svg");
   label->setPixmap(icon.pixmap(150, 150));
}

void Page::Blank::update(const QVariant& data)
{
   Q_UNUSED(data)
   // do nothing
}
