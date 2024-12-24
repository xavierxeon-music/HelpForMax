#include "SuggestModelNamedMessage.h"

Suggest::Model::NamedMessage::NamedMessage(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::MessageNamed)
{
}

void Suggest::Model::NamedMessage::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Name", "Type", "At", "M"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (Ref::Structure::AttributesAndMessageNamed::Map::const_iterator it = suggest.messageNamedMap.constBegin(); it != suggest.messageNamedMap.constEnd(); it++)
   {
      const Ref::Structure::AttributesAndMessageNamed& messageNamed = it.value();

      QStandardItem* nameItem = new QStandardItem();
      nameItem->setEditable(false);
      nameItem->setText(messageNamed.name);
      nameItem->setData(QVariant::fromValue(it));

      QStandardItem* typeItem = new QStandardItem();
      typeItem->setEditable(false);
      typeItem->setText(Max::dataTypeName(messageNamed.dataType));

      QStandardItem* isAttributeItem = new QStandardItem();
      isAttributeItem->setEditable(false);
      isAttributeItem->setCheckable(false);
      isAttributeItem->setIcon(QIcon(":/DocAttribute.svg"));

      if (0 != (messageNamed.patchParts & Ref::Structure::PatchPart::Attribute))
         isAttributeItem->setCheckState(Qt::Checked);
      else
         isAttributeItem->setCheckState(Qt::Unchecked);

      QStandardItem* isMessageItem = new QStandardItem();
      isMessageItem->setEditable(false);
      isMessageItem->setCheckable(false);
      isMessageItem->setIcon(QIcon(":/DocMessageNamed.svg"));

      if (0 != (messageNamed.patchParts & Ref::Structure::PatchPart::MessageNamed))
         isMessageItem->setCheckState(Qt::Checked);
      else
         isMessageItem->setCheckState(Qt::Unchecked);

      invisibleRootItem()->appendRow({nameItem, typeItem, isAttributeItem, isMessageItem});
   }

   endResetModel();
}
