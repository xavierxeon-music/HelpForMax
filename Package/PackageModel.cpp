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

   QDir packageDir(packageInfo->getPath() + "/patchers");
   for (const QFileInfo& folderInfo : packageDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
   {
      QDir patcherDir(folderInfo.canonicalFilePath());
      for (const QFileInfo& patchFileInfo : patcherDir.entryInfoList(QDir::Files, QDir::Name))
      {
         if (!patchFileInfo.fileName().endsWith(".maxpat"))
            continue;

         if (patchFileInfo.fileName().startsWith("_"))
            continue;

         const QString patchPath = patchFileInfo.absoluteFilePath();
         Patch::Info patchInfo = packageInfo->extractPatchInfo(patchPath);

         QString dirName = patchFileInfo.absolutePath();
         int index = dirName.lastIndexOf("/");
         if (index < 0)
            continue;
         dirName = dirName.mid(index + 1);

         if (!folderMap.contains(dirName))
         {
            QStandardItem* folderItem = new QStandardItem(dirName);
            folderItem->setEditable(false);
            invisibleRootItem()->appendRow(folderItem);

            folderMap[dirName] = folderItem;
         }

         QStandardItem* dirItem = folderMap[dirName];

         QStandardItem* patchItem = new QStandardItem(patchInfo.name);
         patchItem->setEditable(false);
         dirItem->appendRow(patchItem);

         patchItem->setData(patchPath, RolePath);
         patchItem->setData(QVariant::fromValue(patchInfo), RoleInfo);
      }
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
         Ref::Structure dummy;
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
   for (int folderRow = 0; folderRow < invisibleRootItem()->rowCount(); folderRow++)
   {
      QStandardItem* folderItem = invisibleRootItem()->child(folderRow, 0);

      for (int patchRow = 0; patchRow < folderItem->rowCount(); patchRow++)
      {
         QStandardItem* patchItem = folderItem->child(patchRow, 0);
         if (patchItem->data(RolePath).toString() == patchFileName)
            return patchItem->index();
      }
   }

   return QModelIndex();
}
