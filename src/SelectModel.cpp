#include "SelectModel.h"

#include <QApplication>
#include <QDir>

#include <ModelItem.h>

#include "MainWindow.h"

SelectModel::SelectModel(MainWindow* mainWindow)
   : QStandardItemModel(mainWindow)
   , FunctionHub()
   , mainWindow(mainWindow)
{
}

void SelectModel::setPackagePath(QString packageDir)
{
   beginResetModel();

   clear();
   setHorizontalHeaderLabels({"Patch"});

   QDir().mkpath(packageDir + "/patchers");
   QDir().mkpath(packageDir + "/docs");

   InfoMap infoMap;
   recursiveSearch(packageDir + "/patchers", infoMap);

   QMap<QString, QStandardItem*> parentMap;
   parentMap[packageDir + "/patchers"] = invisibleRootItem();

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
   for (InfoMap::ConstIterator it = infoMap.constBegin(); it != infoMap.constEnd(); it++)
   {
      const QString key = it.key();
      const QString patchPath = it.value().absoluteFilePath();

      QString itemName = key;
      if (mainWindow->selectPatchStructure(patchPath, key))
         itemName = "* " + itemName;

      ModelItem* patchItem = new ModelItem(itemName);
      patchItem->setData(patchPath, RolePatchPath);
      patchItem->setData(key, RoleKey);

      const QString patchDir = it.value().absolutePath();
      if (!parentMap.contains(patchDir))
      {
         const QString folderName = QString(patchDir).replace(packageDir + "/patchers/", "");

         ModelItem* item = new ModelItem(folderName);
         invisibleRootItem()->appendRow(item);

         parentMap[patchDir] = item;
      }

      parentMap[patchDir]->appendRow(patchItem);
   }
   QApplication::restoreOverrideCursor();

   endResetModel();
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
