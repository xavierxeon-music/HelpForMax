#include "PageBlank.h"

Page::Blank::Blank(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
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
