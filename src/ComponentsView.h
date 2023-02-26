#ifndef ComponentsViewH
#define ComponentsViewH

#include <AbstractItemTreeView.h>

#include <Central.h>

class MainWindow;
class ComponentsModel;

class ComponentsView : public Abstract::ItemTreeView,
                       private FunctionHub
{
   Q_OBJECT

public:
   ComponentsView(MainWindow* mainWindow, ComponentsModel* model);

public slots:
   void slotReloadPatch();
   void slotOpenInExternalEditor();

private:
   void patchSelected(QString patchPath, QString key) override;
   void clicked(ModelItem* item) override;

private:
   MainWindow* mainWindow;
   QString helpPath;
};

#endif // NOT ComponentsViewH
