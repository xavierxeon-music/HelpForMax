#include "SelectModel.h"

#include <QApplication>
#include <QDir>

#include "Tools/ModelItem.h"

#include "MainWindow.h"

SelectModel::SelectModel(MainWindow* mainWindow)
   : QStandardItemModel(mainWindow)
   , FunctionHub()
   , mainWindow(mainWindow)
{
}

void SelectModel::setPackagePath(QString packageDir)
{
   Q_UNUSED(packageDir)

   beginResetModel();

   clear();
   setHorizontalHeaderLabels({"U", "Patch"});

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

   const Block::Item::Map map = mainWindow->getBlockMap();
   for (Block::Item::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      const QString key = it.key();

      ModelItem* undocumntedItem = new ModelItem();

      ModelItem* patchItem = new ModelItem(key);
      patchItem->setData(it.value().getPatchPath(), RolePatchPath);
      patchItem->setData(key, RoleKey);

      invisibleRootItem()->appendRow({undocumntedItem, patchItem});

      if (mainWindow->isBlockUndocumented(key))
         undocumntedItem->setText(" *");
   }

   QApplication::restoreOverrideCursor();

   endResetModel();
}

void SelectModel::setModified(bool enabled, QString key)
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
         if (mainWindow->isBlockUndocumented(rowKey))
            undocumntedItem->setText(" *");
         else
            undocumntedItem->setText("");
      }
   }
}

void SelectModel::recursiveSearch(const QString& path, InfoMap& infoMap)
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
