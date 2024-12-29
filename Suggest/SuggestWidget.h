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
      void slotTransferAll();

   private slots:
      void slotTransferSelected();
      void slotResizeRefColumns();

   private:
      Patch::Widget* patchWidget;
      QList<QTreeView*> refTreeViewList;
   };
} // namespace Suggest

#endif // NOT SuggestWidgetH
