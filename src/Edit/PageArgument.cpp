#include "HelpPageArgument.h"

Help::Page::Argument::Argument(Persona* persona, const PatchParser::Marker& marker)
   : Abstract(persona, marker)
   , highlighter(nullptr)
   , argumentIndex()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("ARGUMENT");
   argumentView->allowNameEdit(true);
}

void Help::Page::Argument::update(const QVariant& data)
{
   argumentIndex = data.toInt();
   PatchStructure::Argument& argument = persona->parserRef().argumentList[argumentIndex];
   keyInfo->setText("argument " + QString::number(argumentIndex) + " @ " + persona->getCurrentKey());

   monitor(digestEdit, &argument.digest.text);
   monitor(descrptionEdit, &argument.digest.description);

   argumentView->clearMonitors();
   argumentView->monitor(&argument);
}
