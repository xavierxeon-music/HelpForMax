#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSplitter>
#include <QToolBar>

#include <Settings.h>

#ifdef Q_OS_MACX
#include <MacTheme.h>
#endif // Q_OS_MACX

#include "ComponentsModel.h"
#include "ComponentsView.h"
#include "EditWidget.h"
#include "SelectModel.h"
#include "SelectView.h"

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

   toolBar->addAction(QIcon(":/New.svg"), "New Package", this, &MainWindow::slotNewPackage);
   toolBar->addAction(QIcon(":/Open.svg"), "Open Package", this, &MainWindow::slotOpenPackage);

   QAction* reloadAction = toolBar->addAction(QIcon(":/Reload.svg"), "Reload Patch", componentsView, &ComponentsView::slotReloadPatch);
   reloadAction->setShortcut(QKeySequence::Refresh);

   QAction* saveAction = toolBar->addAction(QIcon(":/Save.svg"), "Save All Patches", this, &MainWindow::slotSavePatches);
   saveAction->setShortcut(QKeySequence::Save);

   QAction* editorAction = toolBar->addAction(QIcon(":/External.svg"), "Open Patch In External Editor", componentsView, &ComponentsView::slotOpenInExternalEditor);
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
   if (!packagePath.isEmpty())
      callOnAllHubInstances(&Central::setPackagePath, packagePath);
}

void MainWindow::slotNewPackage()
{
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
      callOnAllHubInstances(&Central::setPackagePath, packagePath);
   }
}

void MainWindow::slotSavePatches()
{
   savePatchStructures();
   callOnAllHubInstances(&Central::setModified, false);
}


void MainWindow::setPackagePath(QString packageDir)
{
   if (packageDir.isEmpty())
      setWindowTitle("Help For Max");
   else
      setWindowTitle("Help For Max - [*]" + packageDir);
}

void MainWindow::setModified(bool enabled)
{
   setWindowModified(enabled);
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
