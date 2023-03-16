#include "ComponentsView.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>

#include "ComponentsModel.h"
#include "MainWindow.h"
#include "PatchParser.h"

ComponentsView::ComponentsView(MainWindow* mainWindow, ComponentsModel* model)
   : Abstract::ItemTreeView(mainWindow, model, true)
   , FunctionHub()
   , mainWindow(mainWindow)
   , helpPath()
{
   setHeaderHidden(true);
   setRootIsDecorated(false);
}

void ComponentsView::slotReloadPatch()
{
   mainWindow->parserRef().clear();
   mainWindow->parserRef().load();

   getModel<ComponentsModel>()->patchSelected(QString(), QString());
   callOnOtherHubInstances(&Central::setModified, false, mainWindow->getCurrentKey());
}

void ComponentsView::slotOpenInExternalEditor()
{
   if (helpPath.isEmpty())
      return;

   QDesktopServices::openUrl(QUrl::fromLocalFile(helpPath));
}

void ComponentsView::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(key);

   if (patchPath.isEmpty())
   {
      helpPath = QString();
      return;
   }

   const QFileInfo patchInfo(patchPath);
   const QString patchName = patchInfo.fileName().replace(".maxpat", "");
   const QString packagePath = Central::getPackagePath();

   helpPath = packagePath + "/docs/" + patchName + ".maxref.xml";
}

void ComponentsView::clicked(ModelItem* item)
{
   const QVariant markerVariant = item->data(PatchParser::RoleMarker);
   if (!markerVariant.isValid())
      return;

   const PatchParser::Marker marker = markerVariant.value<PatchParser::Marker>();
   const QVariant data = item->data(PatchParser::RoleData);

   callOnOtherHubInstances(&ComponentsView::componentSelected, marker, data);
}

void ComponentsView::setModified(bool modified, QString key)
{
   if (modified)
      return;

   if (key.isEmpty() || mainWindow->getCurrentKey() == key)
      getModel<ComponentsModel>()->patchSelected(QString(), QString());
}
