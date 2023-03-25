#include "SelectView.h"

#include <QDesktopServices>
#include <QUrl>

#include "MainWindow.h"
#include "SelectModel.h"

SelectView::SelectView(MainWindow* mainWindow, SelectModel* model)
   : Abstract::ItemTreeView(mainWindow, model)
   , FunctionHub()
   , mainWindow(mainWindow)
{
   setHeaderHidden(true);
   setRootIsDecorated(false);
}

void SelectView::clicked(ModelItem* item)
{
   const QVariant data = item->data(SelectModel::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   const QString key = item->data(SelectModel::RoleKey).toString();

   mainWindow->selectBlock(key);

   callOnOtherHubInstances(&SelectView::patchSelected, patchPath, key);
}

void SelectView::doubleClicked(ModelItem* item)
{
   const QVariant data = item->data(SelectModel::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   QDesktopServices::openUrl(QUrl::fromLocalFile(patchPath));
}
