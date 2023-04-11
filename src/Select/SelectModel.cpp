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
   setHorizontalHeaderLabels({"U", "Patch"});

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

   const Block::Item::Map map = central->getBlockMap();
   for (Block::Item::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      const QString key = it.key();

      ModelItem* undocumntedItem = new ModelItem();

      ModelItem* patchItem = new ModelItem(key);
      patchItem->setData(it.value()->getPatchPath(), RolePatchPath);
      patchItem->setData(key, RoleKey);

      invisibleRootItem()->appendRow({undocumntedItem, patchItem});

      if (central->isBlockUndocumented(key))
         undocumntedItem->setText(" *");
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
      QStandardItem* undocumntedItem = invisibleRootItem()->child(row, 0);
      QStandardItem* patchItem = invisibleRootItem()->child(row, 1);

      const QString& rowKey = patchItem->text();

      if (key.isEmpty() || rowKey == key)
      {
         if (central->isBlockUndocumented(rowKey))
            undocumntedItem->setText(" *");
         else
            undocumntedItem->setText("");
      }
   }
}

void Select::Model::recursiveSearch(const QString& path, InfoMap& infoMap)
{
   const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs;
   for (QFileInfo fileInfo : QDir(path).entryInfoList(filters))
   {
      if (fileInfo.isDir())
      {
         recursiveSearch(fileInfo.absoluteFilePath(), infoMap);
         continue;
      }

      if (!fileInfo.fileName().endsWith(".maxpat"))
         continue;

      const QString key = fileInfo.fileName().replace(".maxpat", "");

      infoMap[key] = fileInfo;
   }
}
