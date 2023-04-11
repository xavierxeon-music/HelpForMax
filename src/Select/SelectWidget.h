#ifndef SelectWidgetH
#define SelectWidgetH

#include <QWidget>

class Central;

namespace Select
{
   class Widget : public QWidget
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);
   };
} // namespace Select

#endif // NOT SelectWidgetH
