#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include <QDomElement>

#include "PackageContainer.h"
#include "PatchContainer.h"

#ifdef TEST_CLIENT_AVAILABLE
#include "TestClient.h"
#endif // TEST_CLIENT_AVAILABLE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotCheckDirty();

private:
   void createActions();
   void populateMenuAndToolBar();
   void createToolBar(QDomElement thingElement);
   void createMenu(QDomElement thingElement, QMenu* parentMenu);
   void closeEvent(QCloseEvent* ce) override;
   void toogleDock(QWidget* widget, const QString& name, bool enabled);

private:
   Package::Container* packageWidget;
   Patch::Container* patchWidget;
#ifdef TEST_CLIENT_AVAILABLE
   TestClient* testClient;
#endif // TEST_CLIENT_AVAILABLE
};

#endif // NOT MainWindowH
