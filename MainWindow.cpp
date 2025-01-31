#include "MainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QToolBar>

#include <MessageBar.h>
#include <Shared.h>

#include "PatchWidget.h"

MainWindow::MainWindow()
   : PopulatedMainWindow()
   , packageWidget(nullptr)
   , patchWidget(nullptr)
#ifdef TEST_CLIENT_AVAILABLE
   , testClient(nullptr)
#endif // TEST_CLIENT_AVAILABLE
{
   setWindowTitle("Help For Max [*]");
   setWindowIcon(QIcon(":/HelpForMax.svg"));

   patchWidget = new Patch::Container(this);
   setCentralWidget(patchWidget);

   MessageBar* messageBar = new MessageBar(this);
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

   packageWidget = new Package::Container(this);
   addDock(packageWidget, Qt::LeftDockWidgetArea, "Package");

   connect(patchWidget, &Patch::Container::signalCheckDirty, this, &MainWindow::slotCheckDirty);
   connect(patchWidget, &Patch::Container::signalCheckDirty, packageWidget, &Package::Container::slotCheckDirty);

   connect(patchWidget, &Patch::Container::signalRefWritten, packageWidget, &Package::Container::slotRefWritten);
   connect(packageWidget, &Package::Container::signalCloseAllPatches, patchWidget, &Patch::Container::slotCloseAllPatches);
   connect(packageWidget, &Package::Container::signalPatchSeleted, patchWidget, &Patch::Container::slotShowPatch);

#ifdef TEST_CLIENT_AVAILABLE
   testClient = new TestClient;
   addDock(testClient, Qt::RightDockWidgetArea, "Test");
#endif // TEST_CLIENT_AVAILABLE

   packageWidget->init();
   patchWidget->init();

   createActions();
   packageWidget->createActions();
   patchWidget->createActions();

   populateMenuAndToolBar(":/_MenuAndToolBar.xml");
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
   PopulatedMainWindow::printSettingsLocation();

   QApplication app(argc, argv);
   app.setAttribute(Qt::AA_DontShowIconsInMenus, true);

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
