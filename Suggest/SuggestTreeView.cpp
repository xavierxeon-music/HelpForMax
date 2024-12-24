#include "SuggestTreeView.h"

#include <QTimer>

#include "SuggestModelAbstract.h"

Suggest::TreeView::TreeView(QWidget* parent)
   : QTreeView(parent)
   , model(nullptr)
{
   setRootIsDecorated(false);
   setUniformRowHeights(true);

   setSelectionBehavior(QAbstractItemView::SelectRows);
   setSelectionMode(QAbstractItemView::MultiSelection);
}

void Suggest::TreeView::init(Model::Abstract* model)
{
   this->model = model;

   setModel(model);
   connect(model, &Model::Abstract::signalDataEdited, this, &TreeView::slotResizeColumns);
}

void Suggest::TreeView::transferAll()
{
   QList<int> rowList;
   for (int index = 0; index < model->rowCount(); index++)
      rowList.append(index);

   model->transfer(rowList);
}

void Suggest::TreeView::transferSelected()
{
   QList<int> rowList;
   for (const QModelIndex& index : selectionModel()->selectedRows())
      rowList.append(index.row());

   model->transfer(rowList);
}

void Suggest::TreeView::slotResizeColumns()
{
   auto resizeIternal = [this]()
   {
      for (int col = 0; col < model->columnCount(); col++)
         this->resizeColumnToContents(col);
   };

   QTimer::singleShot(10, this, resizeIternal);
}
