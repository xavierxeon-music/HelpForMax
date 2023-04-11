#include "PageMessageFree.h"

Page::MessageFree::MessageFree(QWidget* parent, Central* central, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
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
   Block::Structure::Message& message = central->blockRef().messageFreeMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + central->getCurrentKey());

   monitor(digestEdit, &message.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &message.digest.description, central->getCurrentKey());

   argumentView->clearMonitors(central->getCurrentKey());
   for (Block::Structure::Argument& argument : message.arguments)
      argumentView->monitor(&argument);
}
