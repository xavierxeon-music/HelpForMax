#include "PageAttribute.h"

#include "MainWindow.h"

Page::Attribute::Attribute(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , attributeName()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("ATTRIBUTE");
}

void Page::Attribute::update(const QVariant& data)
{
   attributeName = data.toString();
   PatchStructure::Attribute& attribute = mainWindow->parserRef().attributeMap[attributeName];
   keyInfo->setText("attribute " + attributeName + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &attribute.digest.text);
   monitor(descrptionEdit, &attribute.digest.description);
}
