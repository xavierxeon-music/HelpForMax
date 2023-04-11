#ifndef ComponentMetaTagWidgetH
#define ComponentMetaTagWidgetH

#include <QWidget>

#include "Tools/Central.h"

namespace Component
{
   class MetaTagWidget : public QWidget
   {
      Q_OBJECT
   public:
      MetaTagWidget(QWidget* parent, Central* central);

   private:
      Central* central;
   };
} // namespace Component

#endif // NOT ComponentMetaTagWidgetH
