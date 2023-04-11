#include "SelectWidget.h"

#include <QFileDialog>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include "Clean/CleanDialog.h"
#include "Clean/CleanModel.h"
#include "SelectModel.h"
#include "SelectView.h"

#include "Tools/Settings.h"

Select::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
{
   Model* selectModel = new Model(this, central);
   View* selectView = new View(this, central, selectModel);

   QToolBar* toolBar = new QToolBar(this);
   toolBar->setMovable(false);

   toolBar->addAction(QIcon(":/OpenPackage.svg"), "Open Package", this, &Widget::slotOpenPackage);
   toolBar->addAction(QIcon(":/ReloadPackage.svg"), "Reload Package", this, &Widget::slotReloadPackage);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(selectView);

   slotReloadPackage();
}

void Select::Widget::slotOpenPackage()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "package", QString(), "package-info.json");
   if (fileName.isEmpty())
      return;

   const QString packagePath = QFileInfo(fileName).absolutePath();

   {
      Settings settings;
      settings.write("LastPackage", packagePath);

      central->readPackageInfo(packagePath);
      central->compileBlockMap(packagePath);

      callOnOtherHubInstances(&FunctionHub::setPackagePath, packagePath);
   }
}

void Select::Widget::slotReloadPackage()
{
   const QString packagePath = Central::getPackagePath();
   if (packagePath.isEmpty())
      return;

   central->readPackageInfo(packagePath);
   central->compileBlockMap(packagePath);
   QTimer::singleShot(1000, this, &Widget::slotCheckUmmatchedFiles);

   if (!packagePath.isEmpty())
      callOnOtherHubInstances(&FunctionHub::setPackagePath, packagePath);
}

void Select::Widget::slotCheckUmmatchedFiles()
{
   Clean::Model model(this, central->getBlockMap().keys());
   if (!model.needCleaning())
      return;

   Clean::Dialog dialog(this, &model);
   if (QDialog::Accepted != dialog.exec())
      return;

   model.apply();
   slotReloadPackage();
}
