#include "EditWidget.h"

#include "Tools/Central.h"

#include "MainWindow.h"

#include "Edit/PageArgument.h"
#include "Edit/PageAttribute.h"
#include "Edit/PageBlank.h"
#include "Edit/PageMessageStandard.h"
#include "Edit/PageMessageUserDefined.h"
#include "Edit/PageOutput.h"
#include "Edit/PagePatch.h"

Edit::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central)
   , FunctionHub()
   , stackWidget(nullptr)
   , editorMap()
{
   toolBarAction(QIcon(":/DocMissing.svg"), "Apply Changes", this, &Widget::slotApplyChanges);

   stackWidget = new QStackedWidget(this);
   setPayload(stackWidget);

   addEditor<Page::Blank>(Block::Item::Marker::Undefined);
   addEditor<Page::Argument>(Block::Item::Marker::Argument);
   addEditor<Page::Attribute>(Block::Item::Marker::Attribute);
   addEditor<Page::MessageStandard>(Block::Item::Marker::MessageStandard);
   addEditor<Page::MessageUserDefined>(Block::Item::Marker::MessageUserDefined);
   addEditor<Page::Output>(Block::Item::Marker::Output);

   Page::Patch* pagePatch = addEditor<Page::Patch>(Block::Item::Marker::Patch);
   connect(pagePatch, &Page::Patch::signalShowMetaTags, this, &Widget::signalShowMetaTags);
   connect(pagePatch, &Page::Patch::signalShowSeeAlso, this, &Widget::signalShowSeeAlso);
}

void Edit::Widget::slotApplyChanges()
{
}

template <typename EditorType>
EditorType* Edit::Widget::addEditor(const Block::Item::Marker& marker)
{
   EditorType* abstract = new EditorType(this, central, marker);
   stackWidget->addWidget(abstract);

   editorMap[marker] = abstract;

   return abstract;
}

void Edit::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   stackWidget->setCurrentWidget(editorMap.value(Block::Item::Marker::Undefined));
}

void Edit::Widget::componentSelected(Block::Item::Marker marker, QVariant data)
{
   Q_UNUSED(data);

   if (!editorMap.contains(marker))
      return;

   stackWidget->setCurrentWidget(editorMap.value(marker));
}
