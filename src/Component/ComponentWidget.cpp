#include "ComponentWidget.h"

#include <QStackedLayout>
#include <QToolBar>
#include <QVBoxLayout>

#include "ComponentMetaTagWidget.h"
#include "ComponentModel.h"
#include "ComponentSeeAlsoWidget.h"
#include "ComponentView.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , FunctionHub()
   , central(central)
   , stackLayout(nullptr)
{
   Model* componentModel = new Model(this, central);
   View* componentView = new View(this, central, componentModel);

   MetaTagWidget* metaTagWidget = new MetaTagWidget(this);
   SeeAlsoWidget* seeAlsoWidget = new SeeAlsoWidget(this);

   QToolBar* toolBar = new QToolBar(this);
   toolBar->setMovable(false);

   QAction* reloadAction = toolBar->addAction(QIcon(":/ReloadPatch.svg"), "Reload Patch", componentView, &View::slotReloadPatch);
   reloadAction->setShortcut(QKeySequence::Refresh);

   QAction* saveAction = toolBar->addAction(QIcon(":/SaveAllPatches.svg"), "Save All Patches", this, &Widget::slotSavePatches);
   saveAction->setShortcut(QKeySequence::Save);

   QAction* editorAction = toolBar->addAction(QIcon(":/Editor.svg"), "Open Patch In External Editor", componentView, &View::slotOpenInExternalEditor);
   editorAction->setShortcut(QKeySequence::Print);

   for (QAction* action : this->findChildren<QAction*>())
   {
      const QString& shortcutName = action->shortcut().toString();
      if (shortcutName.isEmpty())
         continue;

      const QString text = action->text();
      action->setText(text + " (" + shortcutName + ")");
   }

   stackLayout = new QStackedLayout;
   stackLayout->addWidget(componentView);
   stackLayout->addWidget(metaTagWidget);
   stackLayout->addWidget(seeAlsoWidget);

   stackLayout->setCurrentIndex(0);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(toolBar);
   masterLayout->addLayout(stackLayout);
}

void Component::Widget::slotSavePatches()
{
   central->saveBlocks();
   callOnAllHubInstances(&FunctionHub::setModified, false, QString());
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
   if (0 == index || index == stackLayout->currentIndex())
      stackLayout->setCurrentIndex(0);
   else
      stackLayout->setCurrentIndex(index);
}
