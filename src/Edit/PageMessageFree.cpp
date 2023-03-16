#include "PageMessageFree.h"

#include "MainWindow.h"

Page::MessageFree::MessageFree(MainWindow* mainWindow, const PatchParser::Marker& marker)
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
   PatchStructure::Message& message = mainWindow->parserRef().messageFreeMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &message.digest.text, mainWindow->getCurrentKey());
   monitor(descrptionEdit, &message.digest.description, mainWindow->getCurrentKey());

   argumentView->clearMonitors(mainWindow->getCurrentKey());
   for (PatchStructure::Argument& argument : message.arguments)
      argumentView->monitor(&argument);
}
