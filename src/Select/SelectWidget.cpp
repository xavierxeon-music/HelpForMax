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
   : Abstract::Widget(parent, central, "Select patch")
{
   Model* selectModel = new Model(this, central);
   View* selectView = new View(this, central, selectModel);

   toolBarAction(QIcon(":/OpenPackage.svg"), "Open Package", this, &Widget::slotOpenPackage);
   toolBarAction(QIcon(":/ReloadPackage.svg"), "Reload Package", this, &Widget::slotReloadPackage);

   setPayload(selectView);

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
