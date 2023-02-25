#include "HelpPageAttribute.h"

Help::Page::Attribute::Attribute(Persona* persona, const PatchParser::Marker& marker)
   : Abstract(persona, marker)
   , highlighter(nullptr)
   , attributeName()
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("ATTRIBUTE");
}

void Help::Page::Attribute::update(const QVariant& data)
{
   attributeName = data.toString();
   PatchStructure::Attribute& attribute = persona->parserRef().attributeMap[attributeName];
   keyInfo->setText("attribute " + attributeName + " @ " + persona->getCurrentKey());

   monitor(digestEdit, &attribute.digest.text);
   monitor(descrptionEdit, &attribute.digest.description);
}
