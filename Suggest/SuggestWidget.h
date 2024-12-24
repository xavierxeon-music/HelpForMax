#ifndef SuggestWidgetH
#define SuggestWidgetH

#include "ui_SuggestWidget.h"
#include <QWidget>

namespace Suggest
{
   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent);
   };
} // namespace Suggest

#endif // NOT SuggestWidgetH
