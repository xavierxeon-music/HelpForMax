#ifndef OverviewWidgetH
#define OverviewWidgetH

#include "Tools/AbstractWidget.h"

namespace Overview
{
   class Widget : public Abstract::Widget
   {
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotOpenInMax();
   };
} // namespace Overview

#endif // NOT OverviewWidgetH
