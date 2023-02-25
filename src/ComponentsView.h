#ifndef HelpComponentsViewH
#define HelpComponentsViewH

#include <AbstractItemTreeView.h>

#include "HelpPersona.h"

namespace Help
{
   class ComponentsModel;

   class ComponentsView : public Abstract::ItemTreeView,
                          private Persona::FunctionHub
   {
      Q_OBJECT
   public:
      ComponentsView(QWidget* parent, ComponentsModel* model);

   private:
      void clicked(ModelItem* item) override;
   };
} // namespace Help

#endif // NOT HelpComponentsViewH
