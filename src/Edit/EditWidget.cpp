#include "EditWidget.h"

#include "Tools/Central.h"

#include "MainWindow.h"

#include "Edit/PageArgument.h"
#include "Edit/PageAttribute.h"
#include "Edit/PageBlank.h"
#include "Edit/PageMessageFree.h"
#include "Edit/PageMessageStandard.h"
#include "Edit/PageOutput.h"
#include "Edit/PagePatch.h"

Edit::Widget::Widget(QWidget* parent, Central* central)
   : QStackedWidget(parent)
   , FunctionHub()
   , central(central)
   , editorMap()
{
   addEditor<Page::Blank>(Block::Item::Marker::Undefined);
   addEditor<Page::Argument>(Block::Item::Marker::Argument);
   addEditor<Page::Attribute>(Block::Item::Marker::Attribute);
   addEditor<Page::MessageStandard>(Block::Item::Marker::MessageStandard);
   addEditor<Page::MessageFree>(Block::Item::Marker::MessageFree);
   addEditor<Page::Output>(Block::Item::Marker::Output);
   addEditor<Page::Patch>(Block::Item::Marker::Patch);
}

template <typename EditorType>
void Edit::Widget::addEditor(const Block::Item::Marker& marker)
{
   Page::Abstract* abstract = new EditorType(this, central, marker);
   addWidget(abstract);

   editorMap[marker] = abstract;
}

void Edit::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   setCurrentWidget(editorMap.value(Block::Item::Marker::Undefined));
}

void Edit::Widget::componentSelected(Block::Item::Marker marker, QVariant data)
{
   Q_UNUSED(data);

   if (!editorMap.contains(marker))
      return;

   setCurrentWidget(editorMap.value(marker));
}
