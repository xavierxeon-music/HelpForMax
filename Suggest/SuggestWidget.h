#ifndef SuggestWidgetH
#define SuggestWidgetH

#include "ui_SuggestWidget.h"
#include <QWidget>

namespace Patch
{
   class Widget;
}

namespace Suggest
{
   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(Patch::Widget* patchWidget);

   public:
      void update();
      void rebuild();

   private slots:
      void slotTransferAll();
      void slotTransferSelected();

   private:
      Patch::Widget* patchWidget;
   };
} // namespace Suggest

#endif // NOT SuggestWidgetH
