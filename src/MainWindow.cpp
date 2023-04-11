#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>

#ifdef Q_OS_MACX
#include "Tools/MacTheme.h"
#endif // Q_OS_MACX

#include "Component/ComponentWidget.h"
#include "Edit/EditWidget.h"
#include "Select/SelectWidget.h"

#include "Tools/Settings.h"

MainWindow::MainWindow()
   : QSplitter(nullptr)
   , FunctionHub()
   , central()
{
   setWindowTitle("Help For Max");

   Select::Widget* selectWidget = new Select::Widget(this, &central);
   Component::Widget* componentWidget = new Component::Widget(this, &central);
   Edit::Widget* editWidget = new Edit::Widget(this, &central);

   connect(editWidget, &Edit::Widget::signalShowMetaTags, componentWidget, &Component::Widget::slotShowMetaTags);
   connect(editWidget, &Edit::Widget::signalShowSeeAlso, componentWidget, &Component::Widget::slotShowSeeAlso);

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
}

void MainWindow::setModified(bool enabled, QString key)
{
   Q_UNUSED(key)

   setWindowModified(enabled);

   if (enabled)
      central.blockRef().markModified();
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
