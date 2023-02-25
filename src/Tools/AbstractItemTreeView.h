#ifndef AbstractItemTreeViewH
#define AbstractItemTreeViewH

#include <QTreeView>

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include <ModelItem.h>

namespace Abstract
{
   class ItemTreeView : public QTreeView
   {
      Q_OBJECT
   public:
      ItemTreeView(QWidget* parent, QStandardItemModel* model, bool createFilterMdoel = false);

   public:
      template <typename ItemModel>
      ItemModel* getModel();

   protected:
      virtual void clicked(ModelItem* item);
      virtual void doubleClicked(ModelItem* item);
      void updateFilter();

   private slots:
      void slotClicked(const QModelIndex& index);
      void slotDoubleClicked(const QModelIndex& index);
      void slotResizeAllColumns();

   private:
      QModelIndex modelIndex(const QModelIndex& index) const;

   private:
      QStandardItemModel* model;
      FilteredModel* filter;
   };

} // namespace Abstract

#ifndef AbstractItemTreeViewHPP
#include "AbstractItemTreeView.hpp"
#endif // NOT AbstractItemTreeViewHPP

#endif // NOT AbstractItemTreeViewH
