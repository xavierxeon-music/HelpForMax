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

   masterLayout->addWidget(treeView);
   masterLayout->addWidget(applyButton);
}

void Component::SeeAlsoWidget::slotApply()
{
   QStringList seeAlsoList = central->block().patch.seeAlsoList;

   const int rowCount = model->invisibleRootItem()->rowCount();
   for (int row = 0; row < rowCount; row++)
   {
      bool checked = (Qt::Checked == model->invisibleRootItem()->child(row, 0)->checkState());
      if (!checked)
         continue;

      const QString key = model->invisibleRootItem()->child(row, 1)->text();
      if (seeAlsoList.contains(key))
         continue;

      seeAlsoList.append(key);
   }

   central->blockRef().patch.seeAlsoList = seeAlsoList;

   callOnOtherHubInstances(&FunctionHub::componentSelected, Block::Item::Marker::Patch, QVariant(true));
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());

   emit signalShowComponents();
}

void Component::SeeAlsoWidget::componentSelected(Block::Item::Marker marker, QVariant data)
{
   Q_UNUSED(data)

   if (Block::Item::Marker::Patch != marker)
      return;

   model->clear();

   const QStringList& seeAlsoList = central->blockRef().patch.seeAlsoList;

   const Block::Item::Map map = central->getBlockMap();
   for (Block::Item::Map::ConstIterator it = map.constBegin(); it != map.constEnd(); it++)
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
