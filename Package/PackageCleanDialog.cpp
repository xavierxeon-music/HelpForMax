#include "PackageCleanDialog.h"

#include "PackageCleanModel.h"

#include <QFile>

Package::Clean::Dialog::Dialog(QWidget* parent, Model* model)
   : QDialog(parent)
   , model(model)
{
   setupUi(this);
   packageTree->setModel(model);

   connect(cleanupButton, &QPushButton::clicked, this, &Dialog::cleanup);

   for (int index = 0; index < model->columnCount(); index++)
      packageTree->resizeColumnToContents(index);

   progress->setProgress("Cleanup progress", 0);
}

void Package::Clean::Dialog::cleanup()
{
   const QStringList fileList = model->compileFiles();
   progress->setMaxValue(fileList.size());

   for (int index = 0; index < fileList.size(); index++)
   {
      const QString& filePath = fileList.at(index);
      progress->setProgress(filePath, index);
      QFile::remove(filePath);
   }

   accept();
}
