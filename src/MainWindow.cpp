#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QSplitter>

#include <Settings.h>

#ifdef Q_OS_MACX
#include <MacTheme.h>
#endif // Q_OS_MACX

MainWindow::MainWindow()
   : QMainWindow(nullptr)
   , Central::FunctionHub()
{
   setWindowTitle("Help For Max");

   QSplitter* splitter = new QSplitter(this);
   splitter->setObjectName("central_splitter");
   setCentralWidget(splitter);

   Settings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   restoreState(widgetSettings.bytes("State"));

   callOnAllHubInstances(&Central::FunctionHub::laod);
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

   callOnAllHubInstances(&Central::FunctionHub::save);

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
