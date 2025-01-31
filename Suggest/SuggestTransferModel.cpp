#include "SuggestTransferModel.h"

#include <QDir>

#include "FileRef.h"
#include "PackageInfo.h"

Suggest::Transfer::Model::Model(QObject* parent, const Package::Info* packageInfo)
   : QStandardItemModel(parent)
{
   setHorizontalHeaderLabels({"Patch Name"});

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
         //qDebug() << patchInfo.name;

         static Ref::Structure dummy;
         const QString refPath = File::RefXML(packageInfo, dummy).getFilePath(patchInfo);

         const QDateTime patchTime = QFileInfo(patchPath).lastModified();
         const QDateTime refTime = QFileInfo(refPath).lastModified();

         const bool upToDate = refTime >= patchTime;
         if (upToDate)
            continue;

         QStandardItem* patchItem = new QStandardItem(patchInfo.name);
         patchItem->setEditable(false);
         invisibleRootItem()->appendRow(patchItem);
      }
   }
}
