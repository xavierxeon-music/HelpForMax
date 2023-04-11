#ifndef SelectViewH
#define SelectViewH

#include "Tools/AbstractItemTreeView.h"
#include "Tools/Central.h"

namespace Select
{
   class Model;

   class View : public Abstract::ItemTreeView,
                private FunctionHub
   {
      Q_OBJECT
   public:
      View(QWidget* parent, Central* central, Model* model);

   private:
      void clicked(ModelItem* item) override;
      void doubleClicked(ModelItem* item) override;

   private:
      Central* central;
   };
} // namespace Select

#endif // NOT SelectViewH
