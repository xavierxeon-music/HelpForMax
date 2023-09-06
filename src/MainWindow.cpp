#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include "Component/ComponentWidget.h"
#include "Edit/EditWidget.h"
#include "Preview/PreviewWidget.h"
#include "Select/SelectWidget.h"

#include "Tools/Settings.h"

MainWindow::MainWindow()
   : QWidget(nullptr)
   , FunctionHub()
   , splitter(nullptr)
   , central()
{
   setWindowTitle("Help For Max");

   splitter = new QSplitter(this);
   splitter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

   Select::Widget* selectWidget = new Select::Widget(this, &central);
   Preview::Widget* previewWidgeet = new Preview::Widget(this, &central);
   Component::Widget* componentWidget = new Component::Widget(this, &central);
   Edit::Widget* editWidget = new Edit::Widget(this, &central);

   connect(editWidget, &Edit::Widget::signalShowMetaTags, componentWidget, &Component::Widget::slotShowMetaTags);
   connect(editWidget, &Edit::Widget::signalShowSeeAlso, componentWidget, &Component::Widget::slotShowSeeAlso);

   splitter->addWidget(selectWidget);
   splitter->addWidget(previewWidgeet);
   splitter->addWidget(componentWidget);
   splitter->addWidget(editWidget);

   QMenuBar* menuBar = new QMenuBar(this);
   QMenu* manualMenu = menuBar->addMenu("Manual");
   manualMenu->addAction("Write INIT", this, &MainWindow::slotWriteAllInit);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addSpacing(0);
   masterLayout->addWidget(menuBar);
   masterLayout->addWidget(splitter);

   Settings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("State"));
}

void MainWindow::slotWriteAllInit()
{
   central.saveBlocks(Block::Item::Component::Settings);
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
   widgetSettings.write("State", splitter->saveState());

   ce->accept();
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWindow mw;
   mw.show();

   return app.exec();
}
