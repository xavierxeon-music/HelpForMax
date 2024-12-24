#include "PatchRefModelOutput.h"

PatchRef::Model::Output::Output(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::Output)
{
}

void PatchRef::Model::Output::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      QStandardItem* activeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 2);

      const Max::DataType type = typeItem->data().value<Max::DataType>();
      const Ref::Structure::Output& output = structure.outputMap[type];

      activeItem->setCheckState(output.active ? Qt::Checked : Qt::Unchecked);

      updateDigestItem(digestItem, output.digest);
   }

   emit signalDataEdited();
}

void PatchRef::Model::Output::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Active", "Digest"});

   for (const Max::DataType& type : Max::dataTypeList())
   {
      QStandardItem* typeItem = new QStandardItem(Max::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type));

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(true);
      activeItem->setIcon(QIcon(":/DocOutput.svg"));

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);

      invisibleRootItem()->appendRow({typeItem, activeItem, digestItem});
   }

   endResetModel();
   update();
}

Ref::Structure::Digest* PatchRef::Model::Output::getDigest(const QModelIndex& index)
{
   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Max::DataType type = typeItem->data().value<Max::DataType>();

   Ref::Structure::Output& output = structure.outputMap[type];
   return &(output.digest);
}

bool PatchRef::Model::Output::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);

   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Max::DataType type = typeItem->data().value<Max::DataType>();

   Ref::Structure::Output& output = structure.outputMap[type];

   if (Qt::CheckStateRole == role)
   {
      const bool enabled = (Qt::Checked == value.toInt());

      if (1 == index.column())
      {
         output.active = enabled;
         structure.setDirty();
      }
   }

   emit signalDataEdited();
   return result;
}
