#include "ComponentSeeAlsoWidget.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "Tools/ModelItem.h"

Component::SeeAlsoWidget::SeeAlsoWidget(QWidget* parent, Central* central)
   : QWidget(parent)
   , FunctionHub()
   , central(central)
   , model(nullptr)
   , treeView(nullptr)
{
   model = new QStandardItemModel(this);

   treeView = new QTreeView(this);
   treeView->setModel(model);
   treeView->setHeaderHidden(true);
   treeView->setRootIsDecorated(false);

   QPushButton* applyButton = new QPushButton("Apply", this);
   connect(applyButton, &QPushButton::clicked, this, &SeeAlsoWidget::slotApply);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(treeView);
   masterLayout->addWidget(applyButton);
}

void Component::SeeAlsoWidget::slotApply()
{
   QStringList seeAlsoList = central->block().patch.seeAlsoList;

   const int rowCount = model->invisibleRootItem()->rowCount();
   for (int row = 0; row < rowCount; row++)
   {
      bool newChecked = (Qt::Checked == model->invisibleRootItem()->child(row, 0)->checkState());

      const QString key = model->invisibleRootItem()->child(row, 1)->text();
      bool prevChecked = seeAlsoList.contains(key);

      if (newChecked == prevChecked)
         continue;
      else if (newChecked)
         seeAlsoList.append(key);
      else if (prevChecked)
         seeAlsoList.removeAll(key);
   }

   central->blockRef().patch.seeAlsoList = seeAlsoList;

   callOnOtherHubInstances(&FunctionHub::componentSelected, Block::Marker::Patch, QVariant(true));
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());

   emit signalShowComponents();
}

void Component::SeeAlsoWidget::componentSelected(Block::Marker marker, QVariant data)
{
   Q_UNUSED(data)

   if (Block::Marker::Patch != marker)
      return;

   model->clear();

   const QStringList& seeAlsoList = central->blockRef().patch.seeAlsoList;

   const Block::Map map = central->getBlockMap();
   for (Block::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
   {
      const QString key = it.key();

      ModelItem* seeAlsoItem = new ModelItem();
      seeAlsoItem->setCheckable(true);
      if (seeAlsoList.contains(key))
         seeAlsoItem->setCheckState(Qt::Checked);

      ModelItem* patchItem = new ModelItem(key);

      model->invisibleRootItem()->appendRow({seeAlsoItem, patchItem});
   }

   treeView->resizeColumnToContents(0);
   treeView->resizeColumnToContents(1);
}
