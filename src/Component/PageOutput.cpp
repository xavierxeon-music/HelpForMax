#include "PageOutput.h"

Page::Output::Output(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
   , highlighter(nullptr)
   , outputIndex()
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("OUTPUT");
}

void Page::Output::update(const QVariant& data)
{
   outputIndex = data.toInt();
   Structure::Output& output = central->blockRef().outputMap[outputIndex];
   keyInfo->setText("output " + QString::number(outputIndex) + " @ " + central->getCurrentKey());

   monitor(descrptionEdit, &output.digest.description, central->getCurrentKey());
}
