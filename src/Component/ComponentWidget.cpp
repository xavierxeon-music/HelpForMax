#include "ComponentWidget.h"

#include <QSplitter>
#include <QStackedWidget>

#include "ComponentMetaTagWidget.h"
#include "ComponentModel.h"
#include "ComponentSeeAlsoWidget.h"
#include "ComponentView.h"
#include "PageArgument.h"
#include "PageAttribute.h"
#include "PageBlank.h"
#include "PageMessageStandard.h"
#include "PageMessageUserDefined.h"
#include "PageOutput.h"
#include "PagePatch.h"

#include "Tools/Settings.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central, "Edit component")
   , FunctionHub()
   , splitter(nullptr)
   , comonentStackWidget(nullptr)
   , editStackWidget(nullptr)
{
   setMinimumWidth(200);

   comonentStackWidget = new QStackedWidget(this);
   setupComponents();

   editStackWidget = new QStackedWidget(this);
   setupEdit();

   splitter = new QSplitter(this);
   splitter->setOrientation(Qt::Vertical);
   splitter->addWidget(comonentStackWidget);
   splitter->addWidget(editStackWidget);

   setPayload(splitter);
   showShortcuts();
}

void Component::Widget::slotSavePatches()
{
   central->saveBlocks(Block::Item::Component::All);
   callOnAllHubInstances(&FunctionHub::setModified, false, QString());
}

void Component::Widget::slotShowComponents()
{
   setComponentStack(0);
}

void Component::Widget::slotShowMetaTags()
{
   setComponentStack(1);
}

void Component::Widget::slotShowSeeAlso()
{
   setComponentStack(2);
}

void Component::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   setComponentStack(0);

   editStackWidget->setCurrentWidget(editorMap.value(Block::Item::Marker::Undefined));
}

void Component::Widget::componentSelected(Block::Item::Marker marker, QVariant data)
{
   Q_UNUSED(data);

   if (!editorMap.contains(marker))
      return;

   editStackWidget->setCurrentWidget(editorMap.value(marker));
}

void Component::Widget::restoreState()
{
   Settings widgetSettings("ComponentWidget");
   splitter->restoreState(widgetSettings.bytes("State"));
}

void Component::Widget::saveState()
{
   Settings widgetSettings("ComponentWidget");
   widgetSettings.write("State", splitter->saveState());
}

void Component::Widget::setComponentStack(const int& index)
{
   if (0 == index || index == comonentStackWidget->currentIndex())
      comonentStackWidget->setCurrentIndex(0);
   else
      comonentStackWidget->setCurrentIndex(index);
}

void Component::Widget::setupComponents()
{
   Model* componentModel = new Model(this, central);
   View* componentView = new View(this, central, componentModel);

   MetaTagWidget* metaTagWidget = new MetaTagWidget(this, central);
   SeeAlsoWidget* seeAlsoWidget = new SeeAlsoWidget(this, central);

   connect(seeAlsoWidget, &SeeAlsoWidget::signalShowComponents, this, &Widget::slotShowComponents);

   QAction* reloadAction = toolBarAction(QIcon(":/ReloadPatch.svg"), "Reload Patch", componentView, &View::slotReloadPatch);
   reloadAction->setShortcut(QKeySequence::Refresh);

   QAction* saveAction = toolBarAction(QIcon(":/SaveAllPatches.svg"), "Save All Patches", this, &Widget::slotSavePatches);
   saveAction->setShortcut(QKeySequence::Save);

   comonentStackWidget->addWidget(componentView);
   comonentStackWidget->addWidget(metaTagWidget);
   comonentStackWidget->addWidget(seeAlsoWidget);

   comonentStackWidget->setCurrentIndex(0);
}

void Component::Widget::setupEdit()
{
   editStackWidget->setContentsMargins(0, 0, 0, 0);

   addEditor<Page::Blank>(Block::Item::Marker::Undefined);
   addEditor<Page::Argument>(Block::Item::Marker::Argument);
   addEditor<Page::Attribute>(Block::Item::Marker::Attribute);
   addEditor<Page::MessageStandard>(Block::Item::Marker::MessageStandard);
   addEditor<Page::MessageUserDefined>(Block::Item::Marker::MessageUserDefined);
   addEditor<Page::Output>(Block::Item::Marker::Output);

   Page::Patch* pagePatch = addEditor<Page::Patch>(Block::Item::Marker::Patch);
   connect(pagePatch, &Page::Patch::signalShowMetaTags, this, &Widget::slotShowMetaTags);
   connect(pagePatch, &Page::Patch::signalShowSeeAlso, this, &Widget::slotShowSeeAlso);
}

template <typename EditorType>
EditorType* Component::Widget::addEditor(const Block::Item::Marker& marker)
{
   EditorType* abstract = new EditorType(this, central, marker);
   editStackWidget->addWidget(abstract);

   editorMap[marker] = abstract;

   return abstract;
}
