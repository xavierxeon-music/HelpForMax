#include "PatchRefModelTypedMessage.h"

PatchRef::Model::TypedMessage::TypedMessage(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::MessageTyped)
{
}

void PatchRef::Model::TypedMessage::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      QStandardItem* activeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 2);

      const Max::DataType type = typeItem->data().value<Max::DataType>();
      Ref::Structure::MessageTyped& message = structure.messageTypedMap[type];

      activeItem->setCheckState(message.active ? Qt::Checked : Qt::Unchecked);

      updateDigestItem(digestItem, message.digest);
   }

   emit signalDataEdited();
}

void PatchRef::Model::TypedMessage::rebuild()
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
      activeItem->setIcon(QIcon(":/DocMessageTyped.svg"));

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);

      invisibleRootItem()->appendRow({typeItem, activeItem, digestItem});
   }

   endResetModel();
   update();
}

Ref::Structure::Digest* PatchRef::Model::TypedMessage::getDigest(const QModelIndex& index)
{
   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Max::DataType type = typeItem->data().value<Max::DataType>();

   Ref::Structure::MessageTyped& message = structure.messageTypedMap[type];
   return &(message.digest);
}

bool PatchRef::Model::TypedMessage::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);

   QStandardItem* typeItem = invisibleRootItem()->child(index.row(), 0);
   const Max::DataType type = typeItem->data().value<Max::DataType>();

   Ref::Structure::MessageTyped& message = structure.messageTypedMap[type];

   if (Qt::EditRole == role)
   {
      if (0 == index.column())
      {
         message.name = value.toString();
         structure.setDirty();
      }
   }
   else if (Qt::CheckStateRole == role)
   {
      const bool enabled = (Qt::Checked == value.toInt());

      if (2 == index.column())
      {
         if (enabled)
         {
            message.active = true;
            message.digest.text = invisibleRootItem()->child(index.row(), 2)->text();
         }
         else
         {
            message.active = false;
         }

         structure.setDirty();
      }
   }

   emit signalDataEdited();
   return result;
}
