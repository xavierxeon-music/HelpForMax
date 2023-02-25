#include "AbstractItemTreeView.h"

Abstract::ItemTreeView::ItemTreeView(QWidget* parent, QStandardItemModel* model)
   : QTreeView(parent)
   , model(model)
{
   setModel(model);
   connect(this, &QAbstractItemView::clicked, this, &ItemTreeView::slotClicked);
   connect(this, &QAbstractItemView::doubleClicked, this, &ItemTreeView::slotDoubleClicked);

   connect(model, &QAbstractItemModel::modelReset, this, &ItemTreeView::slotResizeAllColumns);
   connect(this, &QTreeView::expanded, this, &ItemTreeView::slotResizeAllColumns);
   connect(this, &QTreeView::collapsed, this, &ItemTreeView::slotResizeAllColumns);

   setExpandsOnDoubleClick(false);
}

void Abstract::ItemTreeView::clicked(ModelItem* item)
{
   // do nothing
   Q_UNUSED(item)
}

void Abstract::ItemTreeView::doubleClicked(ModelItem* item)
{
   // do nothing
   Q_UNUSED(item)
}

void Abstract::ItemTreeView::slotClicked(const QModelIndex& index)
{
   if (!isExpanded(index))
      expand(index);

   QStandardItem* item = model->itemFromIndex(index);
   ModelItem* modelItem = static_cast<ModelItem*>(item);
   clicked(modelItem);
}

void Abstract::ItemTreeView::slotDoubleClicked(const QModelIndex& index)
{
   if (isExpanded(index))
      collapse(index);

   QStandardItem* item = model->itemFromIndex(index);
   ModelItem* modelItem = static_cast<ModelItem*>(item);
   doubleClicked(modelItem);
}

void Abstract::ItemTreeView::slotResizeAllColumns()
{
   resizeColumnToContents(0);
}
