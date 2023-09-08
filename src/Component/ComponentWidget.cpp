#include "ComponentWidget.h"

#include <QStackedWidget>

#include "ComponentMetaTagWidget.h"
#include "ComponentModel.h"
#include "ComponentSeeAlsoWidget.h"
#include "ComponentView.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central)
   , FunctionHub()
   , stackWidget(nullptr)
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

   for (QAction* action : this->findChildren<QAction*>())
   {
      const QString& shortcutName = action->shortcut().toString();
      if (shortcutName.isEmpty())
         continue;

      const QString text = action->text();
      action->setText(text + " (" + shortcutName + ")");
   }

   stackWidget = new QStackedWidget(this);

   stackWidget->addWidget(componentView);
   stackWidget->addWidget(metaTagWidget);
   stackWidget->addWidget(seeAlsoWidget);

   stackWidget->setCurrentIndex(0);

   setPayload(stackWidget);
}

void Component::Widget::slotSavePatches()
{
   central->saveBlocks(Block::Item::Component::All);
   callOnAllHubInstances(&FunctionHub::setModified, false, QString());
}

void Component::Widget::slotShowComponents()
{
   setStack(0);
}

void Component::Widget::slotShowMetaTags()
{
   setStack(1);
}

void Component::Widget::slotShowSeeAlso()
{
   setStack(2);
}

void Component::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   setStack(0);
}

void Component::Widget::setStack(const int& index)
{
   if (0 == index || index == stackWidget->currentIndex())
      stackWidget->setCurrentIndex(0);
   else
      stackWidget->setCurrentIndex(index);
}
