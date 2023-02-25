#include "ComponentsView.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>

#include "ComponentsModel.h"
#include "PatchParser.h"

ComponentsView::ComponentsView(QWidget* parent, ComponentsModel* model)
   : Abstract::ItemTreeView(parent, model, true)
   , FunctionHub()
   , helpPath()
{
   setHeaderHidden(true);
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
