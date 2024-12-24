#include "PackageWidget.h"

#include "PackageModel.h"

Package::Widget::Widget(QWidget* parent, const Info* info)
   : QWidget(parent)
   , info(info)
   , model(nullptr)
{
   setupUi(this);

   model = new Model(this, info);
   model->create();
   packageTree->setModel(model);
   packageTree->setIndentation(0);

   connect(packageTree, &QTreeView::doubleClicked, this, &Widget::slotItemDoubleClicked);
   connect(packageTree, &QTreeView::collapsed, this, &Widget::slotItemCollapsed);
   connect(packageTree, &QTreeView::expanded, this, &Widget::slotItemExpanded);
}

const Package::Info* Package::Widget::getPackageInfo() const
{
   return info;
}

void Package::Widget::selectPatch(const QString& patchFileName)
{
   QModelIndex index = model->find(patchFileName);
   if (!index.isValid())
      return;

   packageTree->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
}

void Package::Widget::checkUpToDate()
{
   model->updateIcons();
}

void Package::Widget::slotItemDoubleClicked(const QModelIndex& index)
{
   QStandardItem* item = model->itemFromIndex(index);
   const QString path = item->data().toString();
   if (path.isEmpty())
      return;

   emit signalPatchSeleted(path, info);
}

void Package::Widget::slotItemCollapsed(const QModelIndex& index)
{
   QStandardItem* item = model->itemFromIndex(index);
   item->setData(false, Model::RoleExpanded);

   model->updateIcons();
}

void Package::Widget::slotItemExpanded(const QModelIndex& index)
{
   QStandardItem* item = model->itemFromIndex(index);
   item->setData(true, Model::RoleExpanded);

   model->updateIcons();
}
