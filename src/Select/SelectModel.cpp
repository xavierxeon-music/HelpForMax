#include "SelectModel.h"

#include <QApplication>
#include <QDir>

#include "Tools/Central.h"
#include "Tools/ModelItem.h"

Select::Model::Model(QObject* parent, Central* central)
   : QStandardItemModel(parent)
   , FunctionHub()
   , central(central)
{
}

void Select::Model::setPackagePath(QString packageDir)
{
   Q_UNUSED(packageDir)

   beginResetModel();

   clear();
   setHorizontalHeaderLabels({"Patch"});

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

   const Block::Item::Map map = central->getBlockMap();

   auto folderName = [&](Block::Item::Map::ConstIterator it)
   {
      QString path = it.value()->getPatchPath();
      path.replace(packageDir + "/patchers/", "");
      int index = path.indexOf("/");

      path = path.mid(0, index);

      return path;
   };

   QMap<QString, QStandardItem*> folderMap;

   for (Block::Item::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      const QString folder = folderName(it);
      if (folderMap.contains(folder))
         continue;

      QStandardItem* folderItem = new QStandardItem(folder);
      folderItem->setEditable(false);
      invisibleRootItem()->appendRow(folderItem);

      folderMap.insert(folder, folderItem);
   }

   for (Block::Item::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      const QString key = it.key();

      ModelItem* patchItem = new ModelItem(key);
      patchItem->setData(it.value()->getPatchPath(), RolePatchPath);
      patchItem->setData(key, RoleKey);

      const QString folder = folderName(it);
      folderMap.value(folder)->appendRow(patchItem);

      if (central->isBlockUndocumented(key))
         patchItem->setForeground(Central::redBrush);
      else
         patchItem->setForeground(Central::blackBrush);
   }

   QApplication::restoreOverrideCursor();

   endResetModel();
}

void Select::Model::setModified(bool enabled, QString key)
{
   if (enabled)
      return;

   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* patchItem = invisibleRootItem()->child(row, 0);

      const QString& rowKey = patchItem->text();

      if (key.isEmpty() || rowKey == key)
      {
         if (central->isBlockUndocumented(rowKey))
            patchItem->setForeground(Central::redBrush);
         else
            patchItem->setForeground(Central::blackBrush);
      }
   }
}
