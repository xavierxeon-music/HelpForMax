#include "PageOutput.h"

#include "MainWindow.h"

Page::Output::Output(MainWindow* mainWindow, const Block::Item::Marker& marker)
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
   Block::Structure::Output& output = mainWindow->blockRef().outputMap[outputIndex];
   keyInfo->setText("output " + QString::number(outputIndex) + " @ " + mainWindow->getCurrentKey());

   monitor(descrptionEdit, &output.digest.description, mainWindow->getCurrentKey());
}
