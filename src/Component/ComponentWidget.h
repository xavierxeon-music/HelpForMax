#ifndef ComponentWidgetH
#define ComponentWidgetH

#include <QWidget>

#include "Tools/Central.h"

namespace Component
{
   class Widget : public QWidget,
                  public FunctionHub
   {
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotSavePatches();

   private:
      Central* central;
   };
} // namespace Component

#endif // NOT ComponentWidgetH
