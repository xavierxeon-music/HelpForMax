#include "ComponentModel.h"

#include "Tools/ModelItem.h"

Component::Model::Model(QObject* parent, Central* central)
   : QStandardItemModel(parent)
   , FunctionHub()
   , central(central)
{
}

void Component::Model::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   rebuild();
   update();
}

void Component::Model::update()
{
   const Block::Item block = central->block();
   int row = 0;

   auto setUdocStatus = [&](QStandardItem* item, const Block::Structure::Base& base, bool append = false)
   {
      if (base.undocumented)
         item->setText("*");
      else if (!append)
         item->setText("");
   };

   {
      QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
      setUdocStatus(udocItem, block.patch.digest);

      QStandardItem* patchDigestItem = invisibleRootItem()->child(row, 2);
      patchDigestItem->setText(block.patch.digest.text);
      row++;
   }

   {
      for (int index = 0; index < block.argumentList.count(); index++)
      {
         const Block::Structure::Argument& argument = block.argumentList.at(index);

         QStandardItem* udocItem = invisibleRootItem()->child(row + index, 0);
         setUdocStatus(udocItem, argument);
         setUdocStatus(udocItem, argument.digest, true);

         QStandardItem* argItem = invisibleRootItem()->child(row + index, 1);
         argItem->setText(argument.name);

         QStandardItem* argDigestItem = invisibleRootItem()->child(row + index, 2);
         argDigestItem->setText(argument.digest.text);
      }
      row += block.argumentList.count();
   }

   {
      for (Block::Structure::Attribute::Map::ConstIterator it = block.attributeMap.constBegin(); it != block.attributeMap.constEnd(); it++)
      {
         const Block::Structure::Attribute& attribute = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, attribute);
         setUdocStatus(udocItem, attribute.digest, true);

         QStandardItem* attrrDigestItem = invisibleRootItem()->child(row, 2);
         attrrDigestItem->setText(attribute.digest.text);
         row++;
      }
   }

   {
      for (const Block::Structure::Type& type : Block::Structure::typeList())
      {
         ModelItem* udocItem = static_cast<ModelItem*>(invisibleRootItem()->child(row, 0));
         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 2);

         if (block.messageStandardMap.contains(type))
         {
            const Block::Structure::Message& message = block.messageStandardMap.value(type);

            msgDigestItem->setText(message.digest.text);
            udocItem->setVisible(true);
            setUdocStatus(udocItem, message);
         }
         else
         {
            udocItem->setVisible(false);
         }

         row++;
      }

      for (Block::Structure::Message::FreeMap::ConstIterator it = block.messageFreeMap.constBegin(); it != block.messageFreeMap.constEnd(); it++)
      {
         const Block::Structure::Message& message = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, message);

         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 2);
         msgDigestItem->setText(message.digest.text);

         row++;
      }
   }

   {
      for (Block::Structure::Output::Map::ConstIterator it = block.outputMap.constBegin(); it != block.outputMap.constEnd(); it++)
      {
         const Block::Structure::Output& output = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, output);
         setUdocStatus(udocItem, output.digest);

         QStandardItem* outputItem = invisibleRootItem()->child(row, 2);
         outputItem->setText(output.name);

         row++;
      }
   }
}

QStandardItem* Component::Model::getItem(const int& row, const int& column) const
{
   int rowCount = 0;
   for (int index = 0; index < invisibleRootItem()->rowCount(); index++)
   {
      ModelItem* front = static_cast<ModelItem*>(invisibleRootItem()->child(index));
      if (!front->isVisible())
         continue;

      if (rowCount == row)
         return invisibleRootItem()->child(index, column);

      rowCount++;
   }

   return nullptr;
}

void Component::Model::setModified(bool enabled, QString key)
{
   Q_UNUSED(key)

   if (enabled)
      update();
}

void Component::Model::rebuild()
{
   beginResetModel();

   clear();

   const Block::Item& block = central->block();

   auto createUDocItem = [](const QString& iconPath, const Block::Item::Marker& marker, const QVariant& data)
   {
      ModelItem* udocItem = new ModelItem();
      udocItem->setIcon(QIcon(iconPath));

      udocItem->setData(QVariant::fromValue(marker), Block::Item::RoleMarker);
      udocItem->setData(data, Block::Item::RoleData);

      return udocItem;
   };

   {
      ModelItem* udocItem = createUDocItem(":/PatchGeneral.svg", Block::Item::Marker::Patch, true);

      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem();

      invisibleRootItem()->appendRow({udocItem, patchItem, patchDigestItem});
   }

   {
      for (int index = 0; index < block.argumentList.count(); index++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchArgument.svg", Block::Item::Marker::Argument, index);

         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, argItem, argDigestItem});
      }
   }

   {
      for (Block::Structure::Attribute::Map::ConstIterator it = block.attributeMap.constBegin(); it != block.attributeMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchAttribute.svg", Block::Item::Marker::Attribute, it.key());

         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, attrItem, attrrDigestItem});
      }
   }

   {
      for (const Block::Structure::Type& type : Block::Structure::typeList())
      {
         ModelItem* udocItem = createUDocItem(":/PatchMessageStandard.svg", Block::Item::Marker::MessageStandard, QVariant::fromValue(type));

         ModelItem* msgItem = new ModelItem(Block::Structure::typeName(type)); // read only
         ModelItem* msgDigestItem = new ModelItem();

         if (block.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(block.messageStandardMap.value(type).digest.text);
         }
         else
         {
            udocItem->setVisible(false);
         }

         invisibleRootItem()->appendRow({udocItem, msgItem, msgDigestItem});
      }

      for (Block::Structure::Message::FreeMap::ConstIterator it = block.messageFreeMap.constBegin(); it != block.messageFreeMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchMessageFree.svg", Block::Item::Marker::MessageFree, it.key());

         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, msgItem, msgDigestItem});
      }
   }

   {
      for (Block::Structure::Output::Map::ConstIterator it = block.outputMap.constBegin(); it != block.outputMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchOutput.svg", Block::Item::Marker::Output, it.key());

         ModelItem* outputNumberItem = new ModelItem("#" + QString::number(it.key())); // read only
         ModelItem* outputItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, outputNumberItem, outputItem});
      }
   }

   endResetModel();
}
