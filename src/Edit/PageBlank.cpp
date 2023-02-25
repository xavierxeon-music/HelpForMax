#include "HelpPageBlank.h"

Help::Page::Blank::Blank(Persona* persona, const PatchParser::Marker& marker)
   : Abstract(persona, marker)
{
   setupUi(this);
}

void Help::Page::Blank::update(const QVariant& data)
{
   Q_UNUSED(data)
   // do nothing
}
