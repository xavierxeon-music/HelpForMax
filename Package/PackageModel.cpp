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

   using Entry = Package::Info::Entry;
   using FolderMap = QMap<QString, QStandardItem*>;
   FolderMap folderMap;

   const Entry::Map& entryMap = packageInfo->getEntryMap();
   for (Entry::Map::const_iterator it = entryMap.constBegin(); it != entryMap.constEnd(); ++it)
   {
      if (it.key().startsWith("_"))
         continue;

      const Entry& entry = it.value();
      const QString patchPath = entry.patchPath;
      if (patchPath.isEmpty())
         continue;

      Patch::Info patchInfo = packageInfo->extractPatchInfo(patchPath);

      if (!folderMap.contains(patchInfo.folder))
      {
         QStandardItem* folderItem = new QStandardItem(patchInfo.folder);
         folderItem->setEditable(false);
         invisibleRootItem()->appendRow(folderItem);

         folderMap[patchInfo.folder] = folderItem;
      }

      QStandardItem* dirItem = folderMap[patchInfo.folder];

      QStandardItem* patchItem = new QStandardItem(patchInfo.name);
      patchItem->setEditable(false);
      dirItem->appendRow(patchItem);

      patchItem->setData(patchPath, RolePath);
      patchItem->setData(QVariant::fromValue(patchInfo), RoleInfo);
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

   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);
      bool folderUpToDate = true;

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);

         const QString path = patchItem->data(RolePath).toString();
         const Patch::Info patchInfo = patchItem->data(RoleInfo).value<Patch::Info>();
         static Ref::Structure dummy;
         const QString refPath = File::RefXML(packageInfo, dummy).getFilePath(patchInfo);

         const QDateTime patchTime = QFileInfo(path).lastModified();
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
   QFileInfo fileInfo(patchFileName);

   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);
         const QString itemPath = patchItem->data(RolePath).toString();
         if (itemPath == fileInfo.canonicalFilePath())
            return patchItem->index();
      }
   }

   return QModelIndex();
}
