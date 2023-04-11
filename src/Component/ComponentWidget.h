#ifndef ComponentWidgetH
#define ComponentWidgetH

#include <QWidget>

#include "Tools/Central.h"

class QStackedLayout;

namespace Component
{
   class Widget : public QWidget,
                  public FunctionHub
   {
   public:
      Widget(QWidget* parent, Central* central);

   public slots:
      void slotShowMetaTags();
      void slotShowSeeAlso();

   private slots:
      void slotSavePatches();

   private:
      void patchSelected(QString patchPath, QString key) override;
      void setStack(const int& index);

   private:
      Central* central;
      QStackedLayout* stackLayout;
   };
} // namespace Component

#endif // NOT ComponentWidgetH
