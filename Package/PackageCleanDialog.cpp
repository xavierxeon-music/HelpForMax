#include "PackageCleanDialog.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "PackageInfo.h"

Package::Clean::Dialog::Dialog(QWidget* parent, const Info* packageInfo)
   : QDialog(parent)
   , packageInfo(packageInfo)
{
   setupUi(this);

   QStandardItemModel* model = createModel();
   packageTree->setModel(model);

   connect(cleanupButton, &QPushButton::clicked, this, &Dialog::cleanup);

   for (int index = 0; index < model->columnCount(); index++)
      packageTree->resizeColumnToContents(index);

   progress->setProgress("Cleanup progress", 0);
}

void Package::Clean::Dialog::cleanup()
{
   const QStringList fileList = packageInfo->compileRedundantFiles();
   progress->setMaxValue(fileList.size());

   for (int index = 0; index < fileList.size(); index++)
   {
      const QString& filePath = fileList.at(index);
      progress->setProgress(filePath, index);
      QFile::remove(filePath);
   }

   accept();
}

QStandardItemModel* Package::Clean::Dialog::createModel()
{
   QStandardItemModel* model = new QStandardItemModel(this);
   model->setHorizontalHeaderLabels({"Name", "Ref", "Help", "Init"});

   using Entry = Package::Info::Entry;
   const Entry::Map& entryMap = packageInfo->getEntryMap();

   for (Entry::Map::const_iterator it = entryMap.constBegin(); it != entryMap.constEnd(); ++it)
   {
      const Entry& entry = it.value();
      if (!entry.patchPath.isEmpty())
         continue;

      QStandardItem* nameItem = new QStandardItem(it.key());
      nameItem->setEditable(false);

      auto checkItem = [&](const QString path)
      {
         QStandardItem* item = new QStandardItem();
         item->setEditable(false);
         item->setCheckable(true);
         item->setCheckState(path.isEmpty() ? Qt::Unchecked : Qt::Checked);

         return item;
      };

      QStandardItem* refItem = checkItem(entry.refPath);
      QStandardItem* helpItem = checkItem(entry.helpPath);
      QStandardItem* initItem = checkItem(entry.initPath);

      model->invisibleRootItem()->appendRow({nameItem, refItem, helpItem, initItem});
   }

   return model;
}
