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

   const Block::Map map = central->getBlockMap();

   using FolderMap = QMap<QString, QStringList>;
   FolderMap folderMap;

   for (Block::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      QString folderName = it.value()->getPatchPath();
      folderName.replace(packageDir + "/patchers/", "");
      int index = folderName.indexOf("/");

      folderName = folderName.mid(0, index);

      if (!folderMap.contains(folderName))
         folderMap[folderName] = QStringList();

      folderMap[folderName].append(it.key());
   }

   for (FolderMap::const_iterator it = folderMap.constBegin(); it != folderMap.constEnd(); it++)
   {
      const QString folderName = it.key();

      QStandardItem* folderItem = new QStandardItem(folderName);
      folderItem->setEditable(false);
      invisibleRootItem()->appendRow(folderItem);

      QStringList patchNameList = it.value();
      patchNameList.sort();

      for (const QString& patchName : patchNameList)
      {
         const Block* block = map[patchName];

         ModelItem* patchItem = new ModelItem(patchName);
         patchItem->setData(block->getPatchPath(), RolePatchPath);
         patchItem->setData(patchName, RoleKey);

         folderItem->appendRow(patchItem);

         if (central->isBlockUndocumented(patchName))
            patchItem->setBackground(Central::udocBrush);
         else
            patchItem->setBackground(Central::docBrush);
      }
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
      QStandardItem* folderItem = invisibleRootItem()->child(row, 0);

      for (int row2 = 0; row2 < folderItem->rowCount(); row2++)
      {
         QStandardItem* patchItem = folderItem->child(row2, 0);
         const QString& rowKey = patchItem->text();

         if (key.isEmpty() || rowKey == key)
         {
            if (central->isBlockUndocumented(rowKey))
               patchItem->setBackground(Central::udocBrush);
            else
               patchItem->setBackground(Central::docBrush);
         }
      }
   }
}
