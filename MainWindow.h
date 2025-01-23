#ifndef MainWindowH
#define MainWindowH

#include <PopulatedMainWindow.h>

#include "PackageContainer.h"
#include "PatchContainer.h"

#ifdef TEST_CLIENT_AVAILABLE
#include "TestClient.h"
#endif // TEST_CLIENT_AVAILABLE

class MainWindow : public PopulatedMainWindow
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotCheckDirty();

private:
   void createActions();

   void toogleDock(QWidget* widget, const QString& name, bool enabled);
   QMenu* createPopupMenu() override;

private:
   Package::Container* packageWidget;
   Patch::Container* patchWidget;
#ifdef TEST_CLIENT_AVAILABLE
   TestClient* testClient;
#endif // TEST_CLIENT_AVAILABLE
};

#endif // NOT MainWindowH
