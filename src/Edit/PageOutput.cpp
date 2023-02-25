#include "PageOutput.h"

#include "MainWindow.h"

Page::Output::Output(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
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
   PatchStructure::Output& output = mainWindow->parserRef().outputMap[outputIndex];
   keyInfo->setText("output " + QString::number(outputIndex) + " @ " + mainWindow->getCurrentKey());

   monitor(descrptionEdit, &output.digest.description);
}
