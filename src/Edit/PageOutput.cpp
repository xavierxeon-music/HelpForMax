#include "HelpPageOutput.h"

Help::Page::Output::Output(Persona* persona, const PatchParser::Marker& marker)
   : Abstract(persona, marker)
   , highlighter(nullptr)
   , outputIndex()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("OUTPUT");
}

void Help::Page::Output::update(const QVariant& data)
{
   outputIndex = data.toInt();
   PatchStructure::Output& output = persona->parserRef().outputMap[outputIndex];
   keyInfo->setText("output " + QString::number(outputIndex) + " @ " + persona->getCurrentKey());

   monitor(descrptionEdit, &output.digest.description);
}
