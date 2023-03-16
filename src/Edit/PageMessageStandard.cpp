#include "PageMessageStandard.h"

#include "MainWindow.h"

Page::MessageStandard::MessageStandard(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , messageType(PatchStructure::Type::Unkown)
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
}

void Page::MessageStandard::update(const QVariant& data)
{
   messageType = data.value<PatchStructure::Type>();
   PatchStructure::Message& message = mainWindow->parserRef().messageStandardMap[messageType];
   keyInfo->setText("messsage " + PatchStructure::typeName(messageType) + " @ " + mainWindow->getCurrentKey());

   monitor(descrptionEdit, &message.digest.description, mainWindow->getCurrentKey());
}
