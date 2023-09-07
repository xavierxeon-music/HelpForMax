#include "SelectView.h"

#include "SelectModel.h"

Select::View::View(QWidget* parent, Central* central, Model* model)
   : Abstract::ItemTreeView(parent, model)
   , FunctionHub()
   , central(central)
{
   setHeaderHidden(true);
   setRootIsDecorated(false);

   connect(model, &QStandardItemModel::modelReset, this, &View::slotHasBeenLoaded);
}

void Select::View::slotHasBeenLoaded()
{
   expandAll();
   setItemsExpandable(false);
}

void Select::View::clicked(ModelItem* item)
{
   const QVariant data = item->data(Model::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   const QString key = item->data(Model::RoleKey).toString();

   central->selectBlock(key);

   callOnOtherHubInstances(&FunctionHub::patchSelected, patchPath, key);
}
