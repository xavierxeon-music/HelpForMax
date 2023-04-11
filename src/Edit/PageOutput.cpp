#include "PageOutput.h"

Page::Output::Output(QWidget* parent, Central* central, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
   , highlighter(nullptr)
   , outputIndex()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("OUTPUT");
}

void Page::Output::update(const QVariant& data)
{
   outputIndex = data.toInt();
   Block::Structure::Output& output = central->blockRef().outputMap[outputIndex];
   keyInfo->setText("output " + QString::number(outputIndex) + " @ " + central->getCurrentKey());

   monitor(descrptionEdit, &output.digest.description, central->getCurrentKey());
}
