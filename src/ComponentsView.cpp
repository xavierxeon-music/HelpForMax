#include "HelpComponentsView.h"

#include "HelpComponentsModel.h"
#include "HelpPatchParser.h"

Help::ComponentsView::ComponentsView(QWidget* parent, ComponentsModel* model)
   : Abstract::ItemTreeView(parent, model)
   , Persona::FunctionHub()
{
   setHeaderHidden(true);
}

void Help::ComponentsView::clicked(ModelItem* item)
{
   const QVariant markerVariant = item->data(PatchParser::RoleMarker);
   if (!markerVariant.isValid())
      return;

   const PatchParser::Marker marker = markerVariant.value<PatchParser::Marker>();
   const QVariant data = item->data(PatchParser::RoleData);

   callOnAllHubInstances(&ComponentsView::componentSelected, marker, data);
}
