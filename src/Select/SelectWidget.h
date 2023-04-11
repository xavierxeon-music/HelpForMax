#ifndef SelectWidgetH
#define SelectWidgetH

#include <QWidget>

#include "Tools/Central.h"

namespace Select
{
   class Widget : public QWidget,
                  private FunctionHub
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotOpenPackage();
      void slotReloadPackage();
      void slotCheckUmmatchedFiles();

   private:
      Central* central;
   };
} // namespace Select

#endif // NOT SelectWidgetH
