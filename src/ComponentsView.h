#ifndef ComponentsViewH
#define ComponentsViewH

#include <AbstractItemTreeView.h>

#include <Central.h>

class ComponentsModel;

class ComponentsView : public Abstract::ItemTreeView,
                       private FunctionHub
{
   Q_OBJECT

public:
   ComponentsView(QWidget* parent, ComponentsModel* model);

public slots:
   void slotOpenInExternalEditor();

private:
   void patchSelected(QString patchPath, QString key) override;
   void clicked(ModelItem* item) override;

private:
   QString helpPath;
};

#endif // NOT ComponentsViewH
