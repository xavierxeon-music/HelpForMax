#include "ComponentView.h"

#include <QContextMenuEvent>
#include <QFileInfo>
#include <QMenu>

#include "ComponentModel.h"
#include "MainWindow.h"

Component::View::Clip::Clip(const Block::Marker& marker)
   : marker(marker)
   , digest{}
   , type(Structure::Type::Anything)
{
}

// main class

Component::View::View(QWidget* parent, Central* central, Model* model)
   : Abstract::ItemTreeView(parent, model, true)
   , FunctionHub()
   , central(central)
   , helpPath()
   , copyAction(nullptr)
   , pasteAction(nullptr)
   , removeAction(nullptr)
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

   removeAction = new QAction(QIcon(), "Remove", this);
   removeAction->setShortcut(QKeySequence::Delete);
   connect(removeAction, &QAction::triggered, this, &Component::View::slotRemove);
   addAction(removeAction);
}

void Component::View::slotReloadPatch()
{
   central->blockRef().clear();
   central->blockRef().load();

   getModel<Model>()->patchSelected(QString(), QString());
   callOnOtherHubInstances(&FunctionHub::setModified, false, central->getCurrentKey());
}

void Component::View::slotCopy()
{
   QStandardItem* compItem = actionItem();
   if (!compItem)
   {
      clip = Clip();
      return;
   }

   const QVariant markerVariant = compItem->data(Block::RoleMarker);
   const Block::Marker marker = markerVariant.value<Block::Marker>();
   clip = Clip(marker);

   const QVariant data = compItem->data(Block::RoleData);

   switch (marker)
   {
      case Block::Marker::Argument:
      {
         const int argumentIndex = data.toInt();
         const Structure::Argument& argument = central->blockRef().argumentList[argumentIndex];

         clip.digest = argument.digest;
         clip.type = argument.type;
         break;
      }
      case Block::Marker::Attribute:
      {
         const QString attributeName = data.toString();
         const Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];

         clip.digest = attribute.digest;
         clip.type = attribute.type;
         break;
      }
      case Block::Marker::MessageStandard:
      {
         const Structure::Type messageType = data.value<Structure::Type>();
         const Structure::Message& message = central->blockRef().messageStandardMap[messageType];

         clip.digest = message.digest;
         break;
      }
      case Block::Marker::MessageUserDefined:
      {
         const QString messageName = data.toString();
         const Structure::Message& message = central->blockRef().messageUserDefinedMap[messageName];

         clip.digest = message.digest;
         break;
      }
      default:
         return;
   }
}

void Component::View::slotPaste()
{
   QStandardItem* compItem = actionItem();
   if (!compItem)
      return;

   const QVariant markerVariant = compItem->data(Block::RoleMarker);
   const Block::Marker targetMarker = markerVariant.value<Block::Marker>();

   const QVariant data = compItem->data(Block::RoleData);

   switch (targetMarker)
   {
      case Block::Marker::Argument:
      {
         const int argumentIndex = data.toInt();
         Structure::Argument& argument = central->blockRef().argumentList[argumentIndex];

         argument.digest = clip.digest;
         if (Structure::Type::Anything != clip.type)
            argument.type = clip.type;

         break;
      }
      case Block::Marker::Attribute:
      {
         const QString attributeName = data.toString();
         Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];

         attribute.digest = clip.digest;
         if (Structure::Type::Anything != clip.type)
            attribute.type = clip.type;

         break;
      }
      case Block::Marker::MessageStandard:
      {
         const Structure::Type messageType = data.value<Structure::Type>();
         Structure::Message& message = central->blockRef().messageStandardMap[messageType];

         message.digest = clip.digest;

         break;
      }
      case Block::Marker::MessageUserDefined:
      {
         const QString messageName = data.toString();
         Structure::Message& message = central->blockRef().messageUserDefinedMap[messageName];

         message.digest = clip.digest;

         break;
      }
      default:
         return;
   }

   callOnOtherHubInstances(&FunctionHub::componentSelected, targetMarker, data);
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
}

void Component::View::slotRemove()
{
   QStandardItem* compItem = actionItem();
   if (!compItem)
   {
      return;
   }

   const QVariant markerVariant = compItem->data(Block::RoleMarker);
   const Block::Marker marker = markerVariant.value<Block::Marker>();

   const QVariant data = compItem->data(Block::RoleData);

   switch (marker)
   {
      case Block::Marker::Output:
      {
         //const Structure::Type messageType = data.value<Structure::Type>();
         qDebug() << "delete output" << compItem->text();
         break;
      }
      default:
         return;
   }
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
   QStandardItem* compItem = getModel<Model>()->item(row); // first column item

   const QVariant markerVariant = compItem->data(Block::RoleMarker);
   const Block::Marker marker = markerVariant.value<Block::Marker>();

   const QVariant data = compItem->data(Block::RoleData);
   callOnOtherHubInstances(&FunctionHub::componentSelected, marker, data);
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
   menu.addSeparator();
   menu.addAction(removeAction);
   menu.exec(event->globalPos());
}

QStandardItem* Component::View::actionItem()
{
   if (!hasFocus())
      return nullptr;

   if (selectedIndexes().empty())
      return nullptr;

   QModelIndex index = selectedIndexes().first();
   QStandardItem* compItem = getModel<Model>()->getItem(index.row());

   //const QVariant markerVariant = compItem->data(Block::RoleMarker);
   //const Block::Marker targetMarker = markerVariant.value<Block::Marker>();

   return compItem;
}
