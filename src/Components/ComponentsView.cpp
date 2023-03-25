#include "ComponentsView.h"

#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMenu>
#include <QUrl>

#include "ComponentsModel.h"
#include "MainWindow.h"

ComponentsView::Clip::Clip(const Block::Item::Marker& marker)
   : marker(marker)
   , digest{}
   , type(Block::Structure::Type::Unkown)
{
}

// main class

ComponentsView::ComponentsView(MainWindow* mainWindow, ComponentsModel* model)
   : Abstract::ItemTreeView(mainWindow, model, true)
   , FunctionHub()
   , mainWindow(mainWindow)
   , helpPath()
   , copyAction()
   , pasteAction()
   , clip()
{
   setHeaderHidden(true);
   setRootIsDecorated(false);
   setContextMenuPolicy(Qt::DefaultContextMenu);

   copyAction = new QAction(QIcon(), "Copy", this);
   copyAction->setShortcut(QKeySequence::Copy);
   connect(copyAction, &QAction::triggered, this, &ComponentsView::slotCopy);
   addAction(copyAction);

   pasteAction = new QAction(QIcon(), "Paste", this);
   pasteAction->setShortcut(QKeySequence::Paste);
   connect(pasteAction, &QAction::triggered, this, &ComponentsView::slotPaste);
   addAction(pasteAction);
}

void ComponentsView::slotReloadPatch()
{
   mainWindow->blockRef().clear();
   mainWindow->blockRef().load();

   getModel<ComponentsModel>()->patchSelected(QString(), QString());
   callOnOtherHubInstances(&Central::setModified, false, mainWindow->getCurrentKey());
}

void ComponentsView::slotOpenInExternalEditor()
{
   if (helpPath.isEmpty())
      return;

   QDesktopServices::openUrl(QUrl::fromLocalFile(helpPath));
}

void ComponentsView::slotCopy()
{
   QStandardItem* uDocItem = actionItem();
   if (!uDocItem)
   {
      clip = Clip();
      return;
   }

   const QVariant markerVariant = uDocItem->data(Block::Item::RoleMarker);
   const Block::Item::Marker marker = markerVariant.value<Block::Item::Marker>();
   clip = Clip(marker);

   qDebug() << __FUNCTION__;
}

void ComponentsView::slotPaste()
{
   QStandardItem* uDocItem = actionItem();
   if (!uDocItem)
      return;

   switch (clip.marker)
   {
      case Block::Item::Marker::Undefined:
      default:
         break;
   }

   qDebug() << __FUNCTION__;
}

void ComponentsView::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(key);

   if (patchPath.isEmpty())
   {
      helpPath = QString();
      return;
   }

   const QFileInfo patchInfo(patchPath);
   const QString patchName = patchInfo.fileName().replace(".maxpat", "");
   const QString packagePath = Central::getPackagePath();

   helpPath = packagePath + "/docs/" + patchName + ".maxref.xml";
}

void ComponentsView::clicked(ModelItem* item)
{
   int row = item->row();
   QStandardItem* uDocItem = getModel<ComponentsModel>()->item(row); // first column item

   const QVariant markerVariant = uDocItem->data(Block::Item::RoleMarker);
   const Block::Item::Marker marker = markerVariant.value<Block::Item::Marker>();

   const QVariant data = uDocItem->data(Block::Item::RoleData);
   callOnOtherHubInstances(&ComponentsView::componentSelected, marker, data);
}

void ComponentsView::setModified(bool modified, QString key)
{
   if (modified)
      return;

   if (key.isEmpty() || mainWindow->getCurrentKey() == key)
      getModel<ComponentsModel>()->patchSelected(QString(), QString());
}

void ComponentsView::contextMenuEvent(QContextMenuEvent* event)
{
   QModelIndex index = indexAt(event->pos());
   if (!index.isValid())
      return;

   QMenu menu(this);
   menu.addAction(copyAction);
   menu.addAction(pasteAction);
   menu.exec(event->globalPos());
}

QStandardItem* ComponentsView::actionItem()
{
   if (!hasFocus())
      return nullptr;

   if (selectedIndexes().empty())
      return nullptr;

   QModelIndex index = selectedIndexes().first();
   QStandardItem* uDocItem = getModel<ComponentsModel>()->item(index.row());

   return uDocItem;
}
