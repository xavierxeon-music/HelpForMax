#include "HelpPageMessage.h"

Help::Page::Message::Message(Persona* persona, const PatchParser::Marker& marker)
   : Abstract(persona, marker)
   , highlighter(nullptr)
   , messageName()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("MESSAGE");
}

void Help::Page::Message::update(const QVariant& data)
{
   messageName = data.toString();
   PatchStructure::Message& message = persona->parserRef().messageFreeMap[messageName];
   keyInfo->setText("messsage " + messageName + " @ " + persona->getCurrentKey());

   monitor(digestEdit, &message.digest.text);
   monitor(descrptionEdit, &message.digest.description);
}
