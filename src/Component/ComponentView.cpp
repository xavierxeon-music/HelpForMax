#include "ComponentView.h"

#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMenu>
#include <QUrl>

#include "ComponentModel.h"
#include "MainWindow.h"

Component::View::Clip::Clip(const Block::Item::Marker& marker)
   : marker(marker)
   , digest{}
   , type(Block::Structure::Type::Unkown)
{
}

// main class

Component::View::View(QWidget* parent, Central* central, Model* model)
   : Abstract::ItemTreeView(parent, model, true)
   , FunctionHub()
   , central(central)
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
   connect(copyAction, &QAction::triggered, this, &Component::View::slotCopy);
   addAction(copyAction);

   pasteAction = new QAction(QIcon(), "Paste", this);
   pasteAction->setShortcut(QKeySequence::Paste);
   connect(pasteAction, &QAction::triggered, this, &Component::View::slotPaste);
   addAction(pasteAction);
}

void Component::View::slotReloadPatch()
{
   central->blockRef().clear();
   central->blockRef().load();

   getModel<Model>()->patchSelected(QString(), QString());
   callOnOtherHubInstances(&Central::setModified, false, central->getCurrentKey());
}

void Component::View::slotOpenInExternalEditor()
{
   if (helpPath.isEmpty())
      return;

   QDesktopServices::openUrl(QUrl::fromLocalFile(helpPath));
}

void Component::View::slotCopy()
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

   const QVariant data = uDocItem->data(Block::Item::RoleData);

   switch (marker)
   {
      case Block::Item::Marker::Argument:
      {
         const int argumentIndex = data.toInt();
         const Block::Structure::Argument& argument = central->blockRef().argumentList[argumentIndex];

         clip.digest = argument.digest;
         clip.type = argument.type;
         break;
      }
      case Block::Item::Marker::Attribute:
      {
         const QString attributeName = data.toString();
         const Block::Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];

         clip.digest = attribute.digest;
         clip.type = attribute.type;
         break;
      }
      case Block::Item::Marker::MessageStandard:
      {
         const Block::Structure::Type messageType = data.value<Block::Structure::Type>();
         const Block::Structure::Message& message = central->blockRef().messageStandardMap[messageType];

         clip.digest = message.digest;
         break;
      }
      case Block::Item::Marker::MessageFree:
      {
         const QString messageName = data.toString();
         const Block::Structure::Message& message = central->blockRef().messageFreeMap[messageName];

         clip.digest = message.digest;
         break;
      }
      default:
         return;
   }
}

void Component::View::slotPaste()
{
   QStandardItem* uDocItem = actionItem();
   if (!uDocItem)
      return;

   const QVariant markerVariant = uDocItem->data(Block::Item::RoleMarker);
   const Block::Item::Marker targetMarker = markerVariant.value<Block::Item::Marker>();

   const QVariant data = uDocItem->data(Block::Item::RoleData);

   switch (targetMarker)
   {
      case Block::Item::Marker::Argument:
      {
         const int argumentIndex = data.toInt();
         Block::Structure::Argument& argument = central->blockRef().argumentList[argumentIndex];

         argument.digest = clip.digest;
         if (Block::Structure::Type::Unkown != clip.type)
            argument.type = clip.type;

         break;
      }
      case Block::Item::Marker::Attribute:
      {
         const QString attributeName = data.toString();
         Block::Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];

         attribute.digest = clip.digest;
         if (Block::Structure::Type::Unkown != clip.type)
            attribute.type = clip.type;

         break;
      }
      case Block::Item::Marker::MessageStandard:
      {
         const Block::Structure::Type messageType = data.value<Block::Structure::Type>();
         Block::Structure::Message& message = central->blockRef().messageStandardMap[messageType];

         message.digest = clip.digest;

         break;
      }
      case Block::Item::Marker::MessageFree:
      {
         const QString messageName = data.toString();
         Block::Structure::Message& message = central->blockRef().messageFreeMap[messageName];

         message.digest = clip.digest;

         break;
      }
      default:
         return;
   }

   callOnOtherHubInstances(&Component::View::componentSelected, targetMarker, data);
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
}

void Component::View::patchSelected(QString patchPath, QString key)
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

void Component::View::clicked(ModelItem* item)
{
   int row = item->row();
   QStandardItem* uDocItem = getModel<Model>()->item(row); // first column item

   const QVariant markerVariant = uDocItem->data(Block::Item::RoleMarker);
   const Block::Item::Marker marker = markerVariant.value<Block::Item::Marker>();

   const QVariant data = uDocItem->data(Block::Item::RoleData);
   callOnOtherHubInstances(&Component::View::componentSelected, marker, data);
}

void Component::View::setModified(bool modified, QString key)
{
   if (modified)
      return;

   if (key.isEmpty() || central->getCurrentKey() == key)
      getModel<Model>()->patchSelected(QString(), QString());
}

void Component::View::contextMenuEvent(QContextMenuEvent* event)
{
   QModelIndex index = indexAt(event->pos());
   if (!index.isValid())
      return;

   QMenu menu(this);
   menu.addAction(copyAction);
   menu.addAction(pasteAction);
   menu.exec(event->globalPos());
}

QStandardItem* Component::View::actionItem()
{
   if (!hasFocus())
      return nullptr;

   if (selectedIndexes().empty())
      return nullptr;

   QModelIndex index = selectedIndexes().first();
   QStandardItem* uDocItem = getModel<Model>()->getItem(index.row());

   //const QVariant markerVariant = uDocItem->data(Block::Item::RoleMarker);
   //const Block::Item::Marker targetMarker = markerVariant.value<Block::Item::Marker>();

   return uDocItem;
}
