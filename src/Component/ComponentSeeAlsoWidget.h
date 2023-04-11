#ifndef ComponentSeeAlsoWidgetH
#define ComponentSeeAlsoWidgetH

#include <QTreeView>

#include <QStandardItemModel>

namespace Component
{
   class SeeAlsoWidget : public QTreeView
   {
      Q_OBJECT
   public:
      SeeAlsoWidget(QWidget* parent);

   public:
      QStandardItemModel* model;
   };
} // namespace Component

#endif // NOT ComponentSeeAlsoWidgetH
