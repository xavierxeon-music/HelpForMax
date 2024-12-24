#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QDomElement>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QToolBar>

#include <Shared.h>

#include "MessageBar.h"
#include "PatchWidget.h"

MainWindow::MainWindow()
   : QMainWindow(nullptr)
   , packageWidget(nullptr)
   , patchWidget(nullptr)
#ifdef TEST_CLIENT_AVAILABLE
   , testClient(nullptr)
#endif // TEST_CLIENT_AVAILABLE
{
   setWindowTitle("Help For Max [*]");
   setWindowIcon(QIcon(":/HelpForMax.svg"));

   patchWidget = new Patch::TabWidget(this);
   setCentralWidget(patchWidget);

   Message::Bar* messageBar = new Message::Bar(this);
   setStatusBar(messageBar);

   auto addDock = [&](QWidget* widget, const Qt::DockWidgetArea& area, const QString& name)
   {
      QDockWidget* dockWidget = new QDockWidget(this);
      dockWidget->setObjectName(name);
      dockWidget->setWidget(widget);
      dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
      dockWidget->setTitleBarWidget(new QWidget());

      addDockWidget(area, dockWidget);
   };

   packageWidget = new Package::TabWidget(this);
   addDock(packageWidget, Qt::LeftDockWidgetArea, "Package");

   connect(patchWidget, &Patch::TabWidget::signalCheckDirty, this, &MainWindow::slotCheckDirty);
   connect(patchWidget, &Patch::TabWidget::signalCheckDirty, packageWidget, &Package::TabWidget::slotCheckDirty);

   connect(patchWidget, &Patch::TabWidget::signalRefWritten, packageWidget, &Package::TabWidget::slotRefWritten);
   connect(packageWidget, &Package::TabWidget::signalCloseAllPatches, patchWidget, &Patch::TabWidget::slotCloseAllPatches);
   connect(packageWidget, &Package::TabWidget::signalPatchSeleted, patchWidget, &Patch::TabWidget::slotShowPatch);

#ifdef TEST_CLIENT_AVAILABLE
   testClient = new TestClient;
   addDock(testClient, Qt::RightDockWidgetArea, "Test");
#endif // TEST_CLIENT_AVAILABLE

   packageWidget->init();
   patchWidget->init();

   createActions();
   packageWidget->createActions();
   patchWidget->createActions();

   populateMenuAndToolBar();

   QSettings settings;
   qDebug() << "SETTINGS @" << settings.fileName();
   restoreGeometry(settings.value("MainWidget/Geometry").toByteArray());
   restoreState(settings.value("MainWidget/State").toByteArray());
}

void MainWindow::slotCheckDirty()
{
   bool dirty = false;
   for (Patch::Widget* widget : findChildren<Patch::Widget*>())
   {
      dirty |= widget->isDirty();
   }

   setWindowModified(dirty);
}

void MainWindow::createActions()
{
   auto addAction = [&](QIcon icon, QString text, QString objectName, auto slotFunction)
   {
      QAction* action = new QAction(icon, text, this);
      action->setObjectName(objectName);
      connect(action, &QAction::triggered, this, slotFunction);

      return action;
   };

   auto addViewToggle = [&](QWidget* widget, const QString& text, QString objectName, const QIcon& icon = QIcon())
   {
      QAction* viewAction = new QAction(icon, text, this);
      viewAction->setObjectName(objectName);
      viewAction->setCheckable(true);

      auto toggleFunction = std::bind(&MainWindow::toogleDock, this, widget, text, std::placeholders::_1);
      connect(viewAction, &QAction::triggered, toggleFunction);

      if (!icon.isNull())
         viewAction->setIcon(icon);

      QSettings dockSettings;
      const bool enabled = dockSettings.value("DockEnabled/" + text).toBool();

      widget->setVisible(enabled);
      viewAction->setChecked(enabled);

      const QSize size = dockSettings.value("DockSize/" + text).toSize();
      if (enabled && size.width() > 0 && size.height() > 0)
         widget->resize(size);

      return viewAction;
   };

   //
   addViewToggle(packageWidget, "Package", "Main.ShowPackage", QIcon(":/PackageGeneral.svg"));

#ifdef TEST_CLIENT_AVAILABLE
   addViewToggle(testClient, "Test Client", "Main.ShowTestClient");
#endif // TEST_CLIENT_AVAILABLE
}

void MainWindow::populateMenuAndToolBar()
{
   QFile file(":/MenuAndToolBar.xml");
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = file.readAll();
   file.close();

   QDomDocument doc;
   QDomDocument::ParseResult result = doc.setContent(content);
   if (!result.errorMessage.isEmpty())
   {
      qWarning() << "unable to read xml" << result.errorMessage;
      return;
   }

   auto createToolBar = [&](QDomElement thingElement)
   {
      const QString name = thingElement.attribute("Name");
      QToolBar* toolBar = addToolBar(name);
      toolBar->setObjectName(name);
      toolBar->setMovable(false);
      toolBar->setIconSize(QSize(24, 24));

      for (QDomElement contentElement = thingElement.firstChildElement(); !contentElement.isNull(); contentElement = contentElement.nextSiblingElement())
      {
         const QString what = contentElement.tagName();
         if ("Action" == what)
         {
            const QString name = contentElement.attribute("Name");
            QAction* action = findChild<QAction*>(name, Qt::FindChildrenRecursively);
            if (action)
               toolBar->addAction(action);
         }
         else if ("Sperator" == what)
         {
            toolBar->addSeparator();
         }
         else if ("Spacer" == what)
         {
            QWidget* widget = new QWidget(this);
            widget->setMinimumWidth(100);
            widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            toolBar->addWidget(widget);
         }
      }
   };

   auto createMenu = [&](QDomElement thingElement)
   {
      const QString name = thingElement.attribute("Name");
      QMenu* menu = menuBar()->addMenu(name);

      for (QDomElement contentElement = thingElement.firstChildElement(); !contentElement.isNull(); contentElement = contentElement.nextSiblingElement())
      {
         const QString what = contentElement.tagName();
         if ("Action" == what)
         {
            const QString name = contentElement.attribute("Name");
            QAction* action = findChild<QAction*>(name, Qt::FindChildrenRecursively);
            if (action)
               menu->addAction(action);
         }
         else if ("Sperator" == what)
         {
            menu->addSeparator();
         }
         else if ("Menu" == what)
         {
            const QString name = contentElement.attribute("Name");
            QMenu* subMenu = findChild<QMenu*>(name, Qt::FindChildrenRecursively);
            if (subMenu)
               menu->addMenu(subMenu);
         }
      }
   };

   const QDomElement rootElement = doc.documentElement();
   for (QDomElement thingElement = rootElement.firstChildElement(); !thingElement.isNull(); thingElement = thingElement.nextSiblingElement())
   {
      const QString what = thingElement.tagName();
      if ("ToolBar" == what)
         createToolBar(thingElement);
      else if ("Menu" == what)
         createMenu(thingElement);
   }
}

void MainWindow::closeEvent(QCloseEvent* ce)
{
   QSettings settings;
   settings.setValue("MainWidget/Geometry", saveGeometry());
   settings.setValue("MainWidget/State", saveState());

   ce->accept();
}

void MainWindow::toogleDock(QWidget* widget, const QString& name, bool enabled)
{
   QSettings dockSettings;
   dockSettings.setValue("DockEnabled/" + name, enabled);
   if (!enabled)
   {
      dockSettings.setValue("DockSize/" + name, widget->size());
      widget->setVisible(false);
   }
   else
   {
      widget->setVisible(true);
      const QSize size = dockSettings.value("DockSize/" + name).toSize();
      if (size.width() > 0 && size.height() > 0)
         widget->resize(size);
   }
}

// main function

int main(int argc, char** argv)
{
   QApplication::setApplicationName("HelpForMax");
   QApplication::setOrganizationDomain("schweinesystem.ddns.org");
   QApplication::setOrganizationName("SchweineSystem");

   QSettings::setDefaultFormat(QSettings::IniFormat);

   QApplication app(argc, argv);

   // only allow one instance
   using HelpForMax = Shared<"HelpForMax">;
   if (HelpForMax::isServerActive())
   {
      QMessageBox::critical(nullptr, "HelpForMax", "Only one running instance allowed");
      return 1;
   }

   MainWindow mw;
   mw.show();

   return app.exec();
}
