#include "PackageCleanDialog.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "PackageInfo.h"

Package::Clean::Dialog::Dialog(QWidget* parent, const Info* packageInfo)
   : QDialog(parent)
   , packageInfo(packageInfo)
   , entryMap()
{
   setupUi(this);

   fillEntryMap(packageInfo->getPath() + "/patchers", true, ".maxpat", &Entry::patchPath);
   fillEntryMap(packageInfo->getPath() + "/help", true, ".maxhelp", &Entry::refPath);
   fillEntryMap(packageInfo->getPath() + "/docs", true, ".maxref.xml", &Entry::helpPath);
   fillEntryMap(packageInfo->getPath() + "/init", false, ".txt", &Entry::initPath);
   fillEntrySources(packageInfo->getPath() + "/source/maxobjects");
   fillEntrySources(packageInfo->getPath() + "/source/maxglobal");

   QStandardItemModel* model = createModel();
   packageTree->setModel(model);

   connect(cleanupButton, &QPushButton::clicked, this, &Dialog::cleanup);

   for (int index = 0; index < model->columnCount(); index++)
      packageTree->resizeColumnToContents(index);

   progress->setProgress("Cleanup progress", 0);
}

void Package::Clean::Dialog::cleanup()
{
   const QStringList fileList = compileFiles();
   progress->setMaxValue(fileList.size());

   for (int index = 0; index < fileList.size(); index++)
   {
      const QString& filePath = fileList.at(index);
      progress->setProgress(filePath, index);
      QFile::remove(filePath);
   }

   accept();
}

QStringList Package::Clean::Dialog::compileFiles() const
{
   QStringList fileList;
   for (Entry::Map::const_iterator it = entryMap.constBegin(); it != entryMap.constEnd(); ++it)
   {
      const Entry& entry = it.value();
      if (!entry.patchPath.isEmpty())
         continue;

      if (!entry.refPath.isEmpty())
         fileList.append(entry.refPath);

      if (!entry.helpPath.isEmpty())
         fileList.append(entry.helpPath);

      if (!entry.initPath.isEmpty())
         fileList.append(entry.initPath);
   }

   return fileList;
}

QStandardItemModel* Package::Clean::Dialog::createModel()
{
   QStandardItemModel* model = new QStandardItemModel(this);

   model->setHorizontalHeaderLabels({"Name", "Ref", "Help", "Init"});

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

void Package::Clean::Dialog::fillEntryMap(const QString& path, const bool scanSubFolders, const QString& extension, Entry::Variable entryVarialble)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      if (contentInfo.isDir() && scanSubFolders)
      {
         fillEntryMap(contentInfo.absoluteFilePath(), false, extension, entryVarialble);
         continue;
      }
      if (!contentInfo.isFile() || !contentInfo.fileName().endsWith(extension))
         continue;

      const QString name = contentInfo.fileName().replace(extension, "");
      Entry& entry = entryMap[name];
      entry.*entryVarialble = contentInfo.absoluteFilePath();
   }
}

void Package::Clean::Dialog::fillEntrySources(const QString& path)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      const QString name = contentInfo.fileName().replace("_tilde", "~");
      Entry& entry = entryMap[name];
      entry.patchPath = contentInfo.absoluteFilePath();
   }
}
