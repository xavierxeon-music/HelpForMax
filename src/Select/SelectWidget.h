#ifndef SelectWidgetH
#define SelectWidgetH

#include "Tools/AbstractWidget.h"

namespace Select
{
   class Widget : public Abstract::Widget,
                  private FunctionHub
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotOpenPackage();
      void slotReloadPackage();
      void slotCheckUmmatchedFiles();
   };
} // namespace Select

#endif // NOT SelectWidgetH
