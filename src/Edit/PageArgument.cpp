#include "PageArgument.h"

Page::Argument::Argument(QWidget* parent, Central* central, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
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
   Block::Structure::Argument& argument = central->blockRef().argumentList[argumentIndex];
   keyInfo->setText("argument " + QString::number(argumentIndex) + " @ " + central->getCurrentKey());

   monitor(digestEdit, &argument.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &argument.digest.description, central->getCurrentKey());

   argumentView->clearMonitors(central->getCurrentKey());
   argumentView->monitor(&argument);
}
