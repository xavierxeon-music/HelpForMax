#include "ComponentsView.h"

#include "ComponentsModel.h"
#include "PatchParser.h"

ComponentsView::ComponentsView(QWidget* parent, ComponentsModel* model)
   : Abstract::ItemTreeView(parent, model)
   , FunctionHub()
{
   setHeaderHidden(true);
}

void ComponentsView::clicked(ModelItem* item)
{
   const QVariant markerVariant = item->data(PatchParser::RoleMarker);
   if (!markerVariant.isValid())
      return;

   const PatchParser::Marker marker = markerVariant.value<PatchParser::Marker>();
   const QVariant data = item->data(PatchParser::RoleData);

   callOnAllHubInstances(&ComponentsView::componentSelected, marker, data);
}
