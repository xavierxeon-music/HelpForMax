#include "CleanModel.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "Tools/Central.h"

Clean::Model::Model(QObject* parent, const QStringList& keyList)
   : QStandardItemModel(parent)
   , targetMap()
{
   setHorizontalHeaderLabels({"Delete", "Old Name", "New Name"});

   const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs;
   auto addKeys = [&](const QString& path, const QString& extension, Target::MemPtr memPtr)
   {
      const QFileInfoList infoList = QDir(path).entryInfoList(filters);
      for (const QFileInfo& fileInfo : infoList)
      {
         if (fileInfo.isDir())
            continue;

         if (!fileInfo.fileName().endsWith(extension))
            continue;

         const QString key = fileInfo.fileName().replace(extension, "");
         if (keyList.contains(key))
            continue;

         targetMap[key].*memPtr = fileInfo.absoluteFilePath();
      }
   };

   const QString packagePath = Central::getPackagePath();
   addKeys(packagePath + "/docs", ".maxref.xml", &Target::docPath);
   addKeys(packagePath + "/help", ".maxhelp", &Target::helpPath);
   addKeys(packagePath + "/init", ".txt", &Target::initPath);

   for (Target::Map::ConstIterator it = targetMap.constBegin(); it != targetMap.constEnd(); it++)
   {
      const QString key = it.key();

      QStandardItem* delItem = new QStandardItem();
      delItem->setEditable(false);
      delItem->setCheckable(true);

      QStandardItem* keyItem = new QStandardItem(key);
      keyItem->setEditable(false);

      QStandardItem* nameItem = new QStandardItem(key);

      invisibleRootItem()->appendRow({delItem, keyItem, nameItem});
   }
}

bool Clean::Model::needCleaning() const
{
   return (0 != invisibleRootItem()->rowCount());
}

void Clean::Model::apply()
{
   auto compilePathList = [&](const QString& key)
   {
      QStringList pathList;
      if (!targetMap.contains(key))
         return pathList;

      const Target& target = targetMap[key];

      if (!target.docPath.isEmpty())
         pathList.append(target.docPath);
      if (!target.helpPath.isEmpty())
         pathList.append(target.helpPath);
      if (!target.initPath.isEmpty())
         pathList.append(target.initPath);

      return pathList;
   };

   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* delItem = invisibleRootItem()->child(row, 0);
      const bool remove = (delItem->checkState() == Qt::Checked);

      QStandardItem* keyItem = invisibleRootItem()->child(row, 1);
      const QString key = keyItem->text();

      QStandardItem* nameItem = invisibleRootItem()->child(row, 2);
      const QString name = nameItem->text();

      for (const QString& path : compilePathList(key))
      {
         if (remove)
         {
            QFile::remove(path);
            //qInfo() << "remove" << path;
         }
         else if (name != key)
         {
            const QString newPath = QString(path).replace(key, name);
            if(QFile::exists(newPath))
                QFile::remove(newPath);

            QFile::rename(path, newPath);
            // qInfo() << "rename" << path << newPath;
         }
      }
   }
}
