#include "PageMessageStandard.h"

Page::MessageStandard::MessageStandard(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
   , highlighter(nullptr)
   , messageType(Structure::Type::Anything)
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
}

void Page::MessageStandard::update(const QVariant& data)
{
   messageType = data.value<Structure::Type>();
   Structure::Message& message = central->blockRef().messageStandardMap[messageType];
   keyInfo->setText("messsage " + Structure::typeName(messageType) + " @ " + central->getCurrentKey());

   monitor(descrptionEdit, &message.digest.description, central->getCurrentKey());
}
