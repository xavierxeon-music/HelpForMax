#include "PatchRefModelHeader.h"

PatchRef::Model::Header::Header(QObject* parent, Ref::Structure& structure)
   : Abstract(parent, structure, Ref::Structure::PatchPart::Header)
{
}

void PatchRef::Model::Header::update()
{
   QStandardItem* typeItem = invisibleRootItem()->child(0, 0);
   QStandardItem* digestItem = invisibleRootItem()->child(0, 1);

   typeItem->setText(Ref::Structure::patchTypeName(structure.header.patcherType));
   updateDigestItem(digestItem, structure.header.digest);

   emit signalDataEdited();
}

void PatchRef::Model::Header::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"PatchType", "Digest"});

   QStandardItem* typeItem = new QStandardItem();

   QStandardItem* digestItem = new QStandardItem();
   //digestItem->setEditable(false);

   invisibleRootItem()->appendRow({typeItem, digestItem});
   endResetModel();

   update();
}

Ref::Structure::Digest* PatchRef::Model::Header::getDigest(const QModelIndex& index)
{
   Q_UNUSED(index)

   return &(structure.header.digest);
}

bool PatchRef::Model::Header::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole == role)
   {
      if (0 == index.column())
      {
         Ref::Structure::PatchType patchType = Ref::Structure::toPatchType(value.toString());
         structure.header.patcherType = patchType;
         structure.setDirty();
      }
      else if (1 == index.column())
      {
         structure.header.digest.text = value.toString();
         emit signalUpdateDigest(index);
      }
   }

   return result;
}

Ref::Structure::PatchType PatchRef::Model::Header::getPatchType(const int index)
{
   Q_UNUSED(index)

   return structure.header.patcherType;
}
