#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QSplitter>
#include <QTimer>
#include <QToolBar>

#ifdef Q_OS_MACX
#include "Tools/MacTheme.h"
#endif // Q_OS_MACX

#include "Clean/CleanDialog.h"
#include "Clean/CleanModel.h"
#include "Components/ComponentsModel.h"
#include "Components/ComponentsView.h"
#include "Edit/EditWidget.h"
#include "Select/SelectModel.h"
#include "Select/SelectView.h"
#include "Tools/JSONModel.h"
#include "Tools/Settings.h"

MainWindow::MainWindow()
   : QMainWindow(nullptr)
   , Central()
{
   setWindowTitle("Help For Max");

   SelectModel* selectModel = new SelectModel(this);
   ComponentsModel* componentsModel = new ComponentsModel(this);

   SelectView* selectView = new SelectView(this, selectModel);
   ComponentsView* componentsView = new ComponentsView(this, componentsModel);
   Edit::Widget* editWidget = new Edit::Widget(this);

   {
      QSplitter* splitter = new QSplitter(this);
      splitter->setObjectName("central_splitter");
      setCentralWidget(splitter);

      splitter->addWidget(selectView);
      splitter->addWidget(componentsView);
      splitter->addWidget(editWidget);
   }

   QToolBar* toolBar = addToolBar("Main");
   toolBar->setObjectName("main_toolbar");
   toolBar->setMovable(false);

   toolBar->addAction(QIcon(":/OpenPackage.svg"), "Open Package", this, &MainWindow::slotOpenPackage);

   QAction* reloadAction = toolBar->addAction(QIcon(":/ReloadPatch.svg"), "Reload Patch", componentsView, &ComponentsView::slotReloadPatch);
   reloadAction->setShortcut(QKeySequence::Refresh);

   QAction* saveAction = toolBar->addAction(QIcon(":/SaveAllPatches.svg"), "Save All Patches", this, &MainWindow::slotSavePatches);
   saveAction->setShortcut(QKeySequence::Save);

   QAction* editorAction = toolBar->addAction(QIcon(":/Editor.svg"), "Open Patch In External Editor", componentsView, &ComponentsView::slotOpenInExternalEditor);
   editorAction->setShortcut(QKeySequence::Print);

   for (QAction* action : this->findChildren<QAction*>())
   {
      const QString& shortcutName = action->shortcut().toString();
      if (shortcutName.isEmpty())
         continue;

      const QString text = action->text();
      action->setText(text + " (" + shortcutName + ")");
   }

   Settings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   restoreState(widgetSettings.bytes("State"));

   // load package
   const QString packagePath = Central::getPackagePath();
   compileBlockMap(packagePath);
   QTimer::singleShot(1000, this, &MainWindow::slotCheckUmmatchedFiles);

   if (!packagePath.isEmpty())
      callOnAllHubInstances(&Central::setPackagePath, packagePath);
}

void MainWindow::slotOpenPackage()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "package", QString(), "package-info.json");
   if (fileName.isEmpty())
      return;

   const QString packagePath = QFileInfo(fileName).absolutePath();

   {
      Settings settings;
      settings.write("LastPackage", packagePath);

      compileBlockMap(packagePath);
      callOnAllHubInstances(&Central::setPackagePath, packagePath);
   }
}

void MainWindow::slotSavePatches()
{
   saveBlocks();
}

void MainWindow::slotCheckUmmatchedFiles()
{
   Clean::Model model(this, getBlockMap().keys());
   if (!model.needCleaning())
      return;

   Clean::Dialog dialog(this, &model);
   if (QDialog::Accepted != dialog.exec())
      return;

   model.apply();
}

void MainWindow::setPackagePath(QString packageDir)
{
   if (packageDir.isEmpty())
      setWindowTitle("Help For Max");
   else
      setWindowTitle("Help For Max - [*]" + packageDir);

   const QString fileName = packageDir + "/package-info.json";

   QJsonObject object = JSON::fromFile(fileName);
   if (object.empty())
   {
      packageAuthor = "";
      packageName = "";
      return;
   }

   packageAuthor = object["author"].toString();
   packageName = object["name"].toString();

   // qDebug() << packageAuthor << packageName;
}

void MainWindow::setModified(bool enabled, QString key)
{
   Q_UNUSED(key)

   setWindowModified(enabled);

   if (enabled)
      blockRef().markModified();
}

void MainWindow::closeEvent(QCloseEvent* ce)
{
   Settings widgetSettings("MainWidget");
   widgetSettings.write("Geometry", saveGeometry());
   widgetSettings.write("State", saveState());

   ce->accept();
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);
#ifdef Q_OS_MACX
   MacTheme::dark();
#endif // Q_OS_MACX

   MainWindow mw;
   mw.show();

   return app.exec();
}
