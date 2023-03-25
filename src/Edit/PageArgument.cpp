#include "PageArgument.h"

#include "MainWindow.h"

Page::Argument::Argument(MainWindow* mainWindow, const Block::Item::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , argumentIndex()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("ARGUMENT");
   argumentView->allowNameEdit(true);
}

void Page::Argument::update(const QVariant& data)
{
   argumentIndex = data.toInt();
   Block::Structure::Argument& argument = mainWindow->blockRef().argumentList[argumentIndex];
   keyInfo->setText("argument " + QString::number(argumentIndex) + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &argument.digest.text, mainWindow->getCurrentKey());
   monitor(descrptionEdit, &argument.digest.description, mainWindow->getCurrentKey());

   argumentView->clearMonitors(mainWindow->getCurrentKey());
   argumentView->monitor(&argument);
}
