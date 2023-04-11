#ifndef ComponentSeeAlsoWidgetH
#define ComponentSeeAlsoWidgetH

#include <QWidget>

#include <QStandardItemModel>
#include <QTreeView>

#include "Tools/Central.h"

namespace Component
{
   class SeeAlsoWidget : public QWidget,
                         protected FunctionHub
   {
      Q_OBJECT
   public:
      SeeAlsoWidget(QWidget* parent, Central* central);

   signals:
      void signalShowComponents();

   private slots:
      void slotApply();

   private:
      void componentSelected(Block::Item::Marker marker, QVariant data) override;

   private:
      Central* central;
      QStandardItemModel* model;
      QTreeView* treeView;
   };
} // namespace Component

#endif // NOT ComponentSeeAlsoWidgetH
