#include "PageMessageFree.h"

#include "MainWindow.h"

Page::MessageFree::MessageFree(MainWindow* mainWindow, const Block::Item::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , messageName()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
   argumentView->allowNameEdit(false);
}

void Page::MessageFree::update(const QVariant& data)
{
   messageName = data.toString();
   Block::Structure::Message& message = mainWindow->blockRef().messageFreeMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &message.digest.text, mainWindow->getCurrentKey());
   monitor(descrptionEdit, &message.digest.description, mainWindow->getCurrentKey());

   argumentView->clearMonitors(mainWindow->getCurrentKey());
   for (Block::Structure::Argument& argument : message.arguments)
      argumentView->monitor(&argument);
}
