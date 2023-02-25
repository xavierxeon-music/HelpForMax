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
   toolBar->setMovable(false);

   toolBar->addAction(QIcon(":/New.svg"), "New", this, &MainWindow::slotNew);
   toolBar->addAction(QIcon(":/Open.svg"), "Open", this, &MainWindow::slotOpen);
   toolBar->addAction(QIcon(":/Reload.svg"), "Reload", this, &MainWindow::slotReload);
   QAction* saveAction = toolBar->addAction(QIcon(":/Save.svg"), "Save", this, &MainWindow::slotSave);
   saveAction->setShortcut(QKeySequence::Save);
   toolBar->addAction(QIcon(":/About.svg"), "Reload", this, &MainWindow::slotAbout);

   Settings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   restoreState(widgetSettings.bytes("State"));

   const QString packagePath = Central::getPackagePath();
   if (!packagePath.isEmpty())
   {
      callOnAllHubInstances(&Central::setPackagePath, packagePath);
   }
}

void MainWindow::slotNew()
{
}

void MainWindow::slotOpen()
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

void MainWindow::slotReload()
{
}

void MainWindow::slotSave()
{
   savePatchStructures();
   callOnAllHubInstances(&Central::setModified, false);
}

void MainWindow::slotAbout()
{
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
