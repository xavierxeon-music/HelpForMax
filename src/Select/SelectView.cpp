#include "SelectView.h"

#include <QDesktopServices>
#include <QUrl>

#include "SelectModel.h"

Select::View::View(QWidget* parent, Central* central, Model* model)
   : Abstract::ItemTreeView(parent, model)
   , FunctionHub()
   , central(central)
{
   setHeaderHidden(true);
   setRootIsDecorated(false);
}

void Select::View::clicked(ModelItem* item)
{
   const QVariant data = item->data(Model::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   const QString key = item->data(Model::RoleKey).toString();

   central->selectBlock(key);

   callOnOtherHubInstances(&View::patchSelected, patchPath, key);
}

void Select::View::doubleClicked(ModelItem* item)
{
   const QVariant data = item->data(Model::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   QDesktopServices::openUrl(QUrl::fromLocalFile(patchPath));
}
