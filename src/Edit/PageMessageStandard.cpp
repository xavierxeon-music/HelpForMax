#include "PageMessageStandard.h"

#include "MainWindow.h"

Page::MessageStandard::MessageStandard(MainWindow* mainWindow, const Block::Item::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , messageType(Block::Structure::Type::Unkown)
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
}

void Page::MessageStandard::update(const QVariant& data)
{
   messageType = data.value<Block::Structure::Type>();
   Block::Structure::Message& message = mainWindow->blockRef().messageStandardMap[messageType];
   keyInfo->setText("messsage " + Block::Structure::typeName(messageType) + " @ " + mainWindow->getCurrentKey());

   monitor(descrptionEdit, &message.digest.description, mainWindow->getCurrentKey());
}
