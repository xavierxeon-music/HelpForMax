#include "PackageModel.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "FileRef.h"
#include "PackageInfo.h"

Package::Model::Model(QObject* parent, const Info* packageInfo)
   : QStandardItemModel(parent)
   , packageInfo(packageInfo)
{
}

void Package::Model::create()
{
   setHorizontalHeaderLabels({"Folder / Patch Name"});

   using FolderMap = QMap<QString, QStandardItem*>;
   FolderMap folderMap;

   const Patch::Info::Map& infoMap = packageInfo->getPatchInfoMap();
   for (Patch::Info::Map::const_iterator it = infoMap.constBegin(); it != infoMap.constEnd(); ++it)
   {
      if (it.key().startsWith("_"))
         continue;

      const Patch::Info& entry = it.value();
      const QString patchPath = entry.patchPath;
      if (patchPath.isEmpty())
         continue;

      Patch::Info patchInfo = packageInfo->findPatchInfo(patchPath);

      if (!folderMap.contains(patchInfo.getFolder()))
      {
         QStandardItem* folderItem = new QStandardItem(patchInfo.getFolder());
         folderItem->setEditable(false);
         invisibleRootItem()->appendRow(folderItem);

         folderMap[patchInfo.getFolder()] = folderItem;
      }

      QStandardItem* dirItem = folderMap[patchInfo.getFolder()];

      QStandardItem* patchItem = new QStandardItem(patchInfo.getName());
      patchItem->setEditable(false);
      dirItem->appendRow(patchItem);

      patchItem->setData(it.key(), RoleKey);
   }

   updateIcons();
}

void Package::Model::updateIcons()
{
   static const QIcon currentIcon(":/HelpCurrent.svg");
   static const QIcon outdatedIcon(":/HelpOutdated.svg");

   static const QIcon closedIcon(":/TreeClosed.svg");
   static const QIcon closedOutdatedIcon(":/TreeClosedOutdated.svg");
   static const QIcon openIcon(":/TreeOpen.svg");

   const Patch::Info::Map& infoMap = packageInfo->getPatchInfoMap();
   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);
      bool folderUpToDate = true;

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);

         const QString key = patchItem->data(RoleKey).toString();
         const Patch::Info patchInfo = infoMap.value(key);

         static Ref::Structure dummy;
         const QString refPath = File::RefXML(packageInfo, dummy).getFilePath(patchInfo);

         const QDateTime patchTime = QFileInfo(patchInfo.patchPath).lastModified();
         const QDateTime refTime = QFileInfo(refPath).lastModified();

         const bool upToDate = refTime >= patchTime;
         if (!upToDate)
            folderUpToDate = false;

         patchItem->setIcon(upToDate ? currentIcon : outdatedIcon);
      }

      const bool expanded = folderItem->data(RoleExpanded).toBool();
      if (expanded)
         folderItem->setIcon(openIcon);
      else if (folderUpToDate)
         folderItem->setIcon(closedIcon);
      else
         folderItem->setIcon(closedOutdatedIcon);
   }
}

QModelIndex Package::Model::find(const QString& patchFileName) const
{
   const QFileInfo fileInfo(patchFileName);
   const Patch::Info::Map& infoMap = packageInfo->getPatchInfoMap();

   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);
         const QString key = patchItem->data(RoleKey).toString();
         const Patch::Info patchInfo = infoMap.value(key);

         if (patchInfo.patchPath == fileInfo.canonicalFilePath())
            return patchItem->index();
      }
   }

   return QModelIndex();
}
