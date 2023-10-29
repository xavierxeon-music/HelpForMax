#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>

#include "Component/ComponentWidget.h"
#include "Overview/OverviewWidget.h"
#include "Result/ResultWidget.h"
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
   Component::Widget* componentWidget = new Component::Widget(this, &central);
   Result::Widget* resultWidget = new Result::Widget(this, &central);
   Overview::Widget* previewWidgeet = new Overview::Widget(this, &central);

   splitter->addWidget(selectWidget);
   splitter->addWidget(componentWidget);
   splitter->addWidget(resultWidget);
   splitter->addWidget(previewWidgeet);

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

   callOnOtherHubInstances(&FunctionHub::restoreState);
}

void MainWindow::slotWriteAllInit()
{
   central.saveBlocks(Block::Component::InitFile);
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

   callOnOtherHubInstances(&FunctionHub::saveState);

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
