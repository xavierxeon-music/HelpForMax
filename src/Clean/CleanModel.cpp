#include "CleanModel.h"

#include <QFile>
#include <QFileInfo>

Clean::Model::Model(QObject* parent, const QStringList& refFileList, const QStringList& helpFileList)
   : QStandardItemModel(parent)
{
   setHorizontalHeaderLabels({"Delete", "Old Name", "New Name"});
   for (const QString& refPath : refFileList)
   {
      const QString key = QFileInfo(refPath).fileName().replace(".maxref.xml", "");

      QStandardItem* delItem = new QStandardItem();
      delItem->setEditable(false);
      delItem->setCheckable(true);
      delItem->setData(refPath, PathRole);
      delItem->setData(key, KeyRole);

      QStandardItem* keyItem = new QStandardItem(key);
      keyItem->setEditable(false);

      QStandardItem* nameItem = new QStandardItem(key);

      invisibleRootItem()->appendRow({delItem, keyItem, nameItem});
   }
}

void Clean::Model::apply()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* delItem = invisibleRootItem()->child(row, 0);
      const QString path = delItem->data(PathRole).toString();
      const QString key = delItem->data(KeyRole).toString();

      const bool remove = (delItem->checkState() == Qt::Checked);

      QStandardItem* nameItem = invisibleRootItem()->child(row, 2);
      const QString name = nameItem->text();

      if (remove)
      {
         QFile::remove(path);
         qInfo() << "remove" << path;
      }
      else if (name != key)
      {
         const QString newPath = QString(path).replace(key, name);
         QFile::rename(path, newPath);
         qInfo() << "rename" << path << newPath;
      }
   }
}
