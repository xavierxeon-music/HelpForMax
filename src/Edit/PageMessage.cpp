#include "PageMessage.h"

#include "MainWindow.h"

Page::Message::Message(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , messageName()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
   argumentView->allowNameEdit(false);
}

void Page::Message::update(const QVariant& data)
{
   messageName = data.toString();
   PatchStructure::Message& message = mainWindow->parserRef().messageFreeMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &message.digest.text);
   monitor(descrptionEdit, &message.digest.description);

   argumentView->clearMonitors();
   for (PatchStructure::Argument& argument : message.arguments)
      argumentView->monitor(&argument);
}
