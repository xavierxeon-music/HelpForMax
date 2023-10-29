#include "PageBlank.h"

Page::Blank::Blank(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   const QIcon icon(":/HelpForMax.svg");
   label->setPixmap(icon.pixmap(150, 150));
}

void Page::Blank::update(const QVariant& data)
{
   Q_UNUSED(data)
   // do nothing
}
