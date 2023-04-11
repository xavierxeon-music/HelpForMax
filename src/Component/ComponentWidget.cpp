#include "ComponentWidget.h"

#include <QToolBar>
#include <QVBoxLayout>

#include "ComponentModel.h"
#include "ComponentView.h"

Component::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
{
   Model* componentModel = new Model(this, central);
   View* componentView = new View(this, central, componentModel);

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

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(componentView);
}

void Component::Widget::slotSavePatches()
{
   central->saveBlocks();
}
