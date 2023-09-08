#ifndef ComponentWidgetH
#define ComponentWidgetH

#include "Tools/AbstractWidget.h"

class QStackedWidget;

namespace Component
{
   class Widget : public Abstract::Widget,
                  public FunctionHub
   {
   public:
      Widget(QWidget* parent, Central* central);

   public slots:
      void slotShowComponents();
      void slotShowMetaTags();
      void slotShowSeeAlso();

   private slots:
      void slotSavePatches();

   private:
      void patchSelected(QString patchPath, QString key) override;
      void setStack(const int& index);

   private:
      QStackedWidget* stackWidget;
   };
} // namespace Component

#endif // NOT ComponentWidgetH
