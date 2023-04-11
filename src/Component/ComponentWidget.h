#ifndef ComponentWidgetH
#define ComponentWidgetH

#include <QWidget>

class Central;

namespace Component
{
   class Widget : public QWidget
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
