#include "SelectView.h"

#include <QApplication>
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
}

void SelectView::clicked(ModelItem* item)
{
   const QVariant data = item->data(SelectModel::RolePatchPath);
   if (!data.isValid())
      return;

   const QString patchPath = data.toString();
   const QString key = item->data(SelectModel::RoleKey).toString();

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
   mainWindow->buildPatchStructure(patchPath, key);
   QApplication::restoreOverrideCursor();

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
