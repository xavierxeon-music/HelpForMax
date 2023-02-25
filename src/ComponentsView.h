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

private:
   void clicked(ModelItem* item) override;
};

#endif // NOT ComponentsViewH
