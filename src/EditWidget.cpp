#include "EditWidget.h"

#include <Central.h>

#include "MainWindow.h"

#include "Edit/PageArgument.h"
#include "Edit/PageAttribute.h"
#include "Edit/PageBlank.h"
#include "Edit/PageMessage.h"
#include "Edit/PageOutput.h"
#include "Edit/PagePatch.h"

Edit::Widget::Widget(MainWindow* mainWindow)
   : QStackedWidget(mainWindow)
   , FunctionHub()
   , mainWindow(mainWindow)
   , editorMap()
{
   addEditor<Page::Blank>(PatchParser::Marker::Undefined);
   addEditor<Page::Argument>(PatchParser::Marker::Argument);
   addEditor<Page::Attribute>(PatchParser::Marker::Attribute);
   addEditor<Page::Message>(PatchParser::Marker::MessageStandard);
   addEditor<Page::Message>(PatchParser::Marker::MessageFree);
   addEditor<Page::Output>(PatchParser::Marker::Output);
   addEditor<Page::Patch>(PatchParser::Marker::Patch);
}

template <typename EditorType>
void Edit::Widget::addEditor(const PatchParser::Marker& marker)
{
   Page::Abstract* abstract = new EditorType(mainWindow, marker);
   addWidget(abstract);

   editorMap[marker] = abstract;
}

void Edit::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   setCurrentWidget(editorMap.value(PatchParser::Marker::Undefined));
}

void Edit::Widget::componentSelected(PatchParser::Marker marker, QVariant data)
{
   Q_UNUSED(data);

   if (!editorMap.contains(marker))
      return;

   setCurrentWidget(editorMap.value(marker));
}
