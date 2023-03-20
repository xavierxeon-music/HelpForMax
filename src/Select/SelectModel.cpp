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
   beginResetModel();

   clear();
   setHorizontalHeaderLabels({"U", "Patch"});

   QStringList unmatchedRefFileList;
   QDir().mkpath(packageDir + "/docs");
   const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files;
   for (QFileInfo fileInfo : QDir(packageDir + "/docs").entryInfoList(filters))
   {
      if (!fileInfo.fileName().endsWith(".maxref.xml"))
         continue;

      unmatchedRefFileList.append(fileInfo.absoluteFilePath());
   }

   InfoMap infoMap;
   QDir().mkpath(packageDir + "/patchers");
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

      const QString refPath = mainWindow->getRefPath(key);
      if (unmatchedRefFileList.contains(refPath))
         unmatchedRefFileList.removeAll(refPath);
   }

   if (!unmatchedRefFileList.isEmpty())
      emit signalUnmatchedFiles(unmatchedRefFileList, QStringList());

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
         if (mainWindow->isPatchStructureUndocumented(rowKey))
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
