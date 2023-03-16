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
   setHorizontalHeaderLabels({"U", "Patch"});

   QDir().mkpath(packageDir + "/patchers");
   QDir().mkpath(packageDir + "/docs");

   InfoMap infoMap;
   recursiveSearch(packageDir + "/patchers", infoMap);

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
   for (InfoMap::ConstIterator it = infoMap.constBegin(); it != infoMap.constEnd(); it++)
   {
      const QString key = it.key();
      const QString patchPath = it.value().absoluteFilePath();

      ModelItem* undocumntedItem = new ModelItem();

      ModelItem* patchItem = new ModelItem(key);
      patchItem->setData(patchPath, RolePatchPath);
      patchItem->setData(key, RoleKey);

      invisibleRootItem()->appendRow({undocumntedItem, patchItem});

      mainWindow->loadPatchStructure(patchPath, key);

      if (mainWindow->isPatchStructureUndocumented(key))
         undocumntedItem->setText(" *");
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
