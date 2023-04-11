#include "PageMessageStandard.h"

Page::MessageStandard::MessageStandard(QWidget* parent, Central* cental, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
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
   Block::Structure::Message& message = central->blockRef().messageStandardMap[messageType];
   keyInfo->setText("messsage " + Block::Structure::typeName(messageType) + " @ " + central->getCurrentKey());

   monitor(descrptionEdit, &message.digest.description, central->getCurrentKey());
}
