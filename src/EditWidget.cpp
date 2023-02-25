#include "HelpEditWidget.h"

#include "HelpPersona.h"

#include "HelpPageArgument.h"
#include "HelpPageAttribute.h"
#include "HelpPageBlank.h"
#include "HelpPageMessage.h"
#include "HelpPageOutput.h"
#include "HelpPagePatch.h"

Help::Edit::Widget::Widget(Persona* persona)
   : QStackedWidget(persona)
   , Persona::FunctionHub()
   , persona(persona)
   , editorMap()
{
   addEditor<Page::Blank>(PatchParser::Marker::Undefined);
   addEditor<Page::Argument>(PatchParser::Marker::Argument);
   addEditor<Page::Attribute>(PatchParser::Marker::Attribute);
   addEditor<Page::Message>(PatchParser::Marker::Message);
   addEditor<Page::Output>(PatchParser::Marker::Output);
   addEditor<Page::Patch>(PatchParser::Marker::Patch);
}

template <typename EditorType>
void Help::Edit::Widget::addEditor(const PatchParser::Marker& marker)
{
   Page::Abstract* abstract = new EditorType(persona, marker);
   addWidget(abstract);

   editorMap[marker] = abstract;
}

void Help::Edit::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   setCurrentWidget(editorMap.value(PatchParser::Marker::Undefined));
}

void Help::Edit::Widget::componentSelected(PatchParser::Marker marker, QVariant data)
{
   Q_UNUSED(data);

   if (!editorMap.contains(marker))
      return;

   setCurrentWidget(editorMap.value(marker));
}
