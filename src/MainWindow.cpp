#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>

#ifdef Q_OS_MACX
#include "Tools/MacTheme.h"
#endif // Q_OS_MACX

#include "Component/ComponentWidget.h"
#include "Edit/EditWidget.h"
#include "Select/SelectWidget.h"
#include "Tools/JSONModel.h"
#include "Tools/Settings.h"

MainWindow::MainWindow()
   : QSplitter(nullptr)
   , Central()
{
   setWindowTitle("Help For Max");

   Select::Widget* selectWidget = new Select::Widget(this, this);
   Component::Widget* componentWidget = new Component::Widget(this, this);
   Edit::Widget* editWidget = new Edit::Widget(this);

   addWidget(selectWidget);
   addWidget(componentWidget);
   addWidget(editWidget);

   Settings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   restoreState(widgetSettings.bytes("State"));
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
