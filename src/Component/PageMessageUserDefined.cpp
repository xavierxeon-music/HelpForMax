#include "PageMessageUserDefined.h"

Page::MessageUserDefined::MessageUserDefined(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
   , highlighter(nullptr)
   , messageName()
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
   argumentView->allowNameEdit(false);
}

void Page::MessageUserDefined::update(const QVariant& data)
{
   messageName = data.toString();
   Structure::Message& message = central->blockRef().messageUserDefinedMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + central->getCurrentKey());

   monitor(digestEdit, &message.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &message.digest.description, central->getCurrentKey());

   argumentView->clearMonitors(central->getCurrentKey());
   for (Structure::Argument& argument : message.arguments)
      argumentView->monitor(&argument);
}
