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
   const Block block = central->block();
   int row = 0;

   auto setUdocStatus = [&](int row, const Structure::Base& base, bool append = false)
   {
      QStandardItem* itemA = invisibleRootItem()->child(row, 0);
      QStandardItem* itemB = invisibleRootItem()->child(row, 1);

      if (base.undocumented)
      {
         itemA->setBackground(Central::udocBrush);
         itemB->setBackground(Central::udocBrush);
      }
      else if (!append)
      {
         itemA->setBackground(Central::docBrush);
         itemB->setBackground(Central::docBrush);
      }
   };

   {
      setUdocStatus(row, block.patch.digest);

      QStandardItem* patchDigestItem = invisibleRootItem()->child(row, 1);
      patchDigestItem->setText(block.patch.digest.text);
      row++;
   }

   {
      for (int index = 0; index < block.argumentList.count(); index++)
      {
         const Structure::Argument& argument = block.argumentList.at(index);

         setUdocStatus(row + index, argument);
         setUdocStatus(row + index, argument.digest, true);

         QStandardItem* argItem = invisibleRootItem()->child(row + index, 0);
         argItem->setText(argument.name);

         QStandardItem* argDigestItem = invisibleRootItem()->child(row + index, 1);
         argDigestItem->setText(argument.digest.text);
      }
      row += block.argumentList.count();
   }

   {
      for (Structure::Attribute::Map::ConstIterator it = block.attributeMap.constBegin(); it != block.attributeMap.constEnd(); it++)
      {
         const Structure::Attribute& attribute = it.value();

         setUdocStatus(row, attribute);
         setUdocStatus(row, attribute.digest, true);

         QStandardItem* attrrDigestItem = invisibleRootItem()->child(row, 1);
         attrrDigestItem->setText(attribute.digest.text);
         row++;
      }
   }

   {
      for (const Structure::Type& type : Structure::typeList())
      {
         ModelItem* msgDigestItem = static_cast<ModelItem*>(invisibleRootItem()->child(row, 1));

         if (block.messageStandardMap.contains(type))
         {
            const Structure::Message& message = block.messageStandardMap.value(type);
            setUdocStatus(row, message);

            msgDigestItem->setText(message.digest.text);
            msgDigestItem->setVisible(true);
         }
         else
         {
            msgDigestItem->setVisible(false);
         }

         row++;
      }

      for (Structure::Message::FreeMap::ConstIterator it = block.messageUserDefinedMap.constBegin(); it != block.messageUserDefinedMap.constEnd(); it++)
      {
         const Structure::Message& message = it.value();

         setUdocStatus(row, message);

         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 1);
         msgDigestItem->setText(message.digest.text);

         row++;
      }
   }

   {
      for (Structure::Output::Map::ConstIterator it = block.outputMap.constBegin(); it != block.outputMap.constEnd(); it++)
      {
         const Structure::Output& output = it.value();

         setUdocStatus(row, output);
         setUdocStatus(row, output.digest, true);

         QStandardItem* outputItem = invisibleRootItem()->child(row, 1);
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

   const Block& block = central->block();

   auto tagRow = [](ModelItem* firstItem, const QString& iconPath, const Block::Marker& marker, const QVariant& data)
   {
      firstItem->setIcon(QIcon(iconPath));

      firstItem->setData(QVariant::fromValue(marker), Block::RoleMarker);
      firstItem->setData(data, Block::RoleData);
   };

   {
      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem();

      tagRow(patchItem, ":/PatchGeneral.svg", Block::Marker::Patch, true);
      invisibleRootItem()->appendRow({patchItem, patchDigestItem});
   }

   {
      for (int index = 0; index < block.argumentList.count(); index++)
      {
         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         tagRow(argItem, ":/PatchArgument.svg", Block::Marker::Argument, index);
         invisibleRootItem()->appendRow({argItem, argDigestItem});
      }
   }

   {
      for (Structure::Attribute::Map::ConstIterator it = block.attributeMap.constBegin(); it != block.attributeMap.constEnd(); it++)
      {
         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         tagRow(attrItem, ":/PatchAttribute.svg", Block::Marker::Attribute, it.key());
         invisibleRootItem()->appendRow({attrItem, attrrDigestItem});
      }
   }

   {
      for (const Structure::Type& type : Structure::typeList())
      {
         ModelItem* msgItem = new ModelItem(Structure::typeName(type)); // read only
         ModelItem* msgDigestItem = new ModelItem();

         if (block.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(block.messageStandardMap.value(type).digest.text);
            msgItem->setVisible(true);
         }
         else
         {
            msgItem->setVisible(false);
         }

         tagRow(msgItem, ":/PatchMessageStandard.svg", Block::Marker::MessageStandard, QVariant::fromValue(type));
         invisibleRootItem()->appendRow({msgItem, msgDigestItem});
      }

      for (Structure::Message::FreeMap::ConstIterator it = block.messageUserDefinedMap.constBegin(); it != block.messageUserDefinedMap.constEnd(); it++)
      {
         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         tagRow(msgItem, ":/PatchMessageUserDefined.svg", Block::Marker::MessageUserDefined, it.key());
         invisibleRootItem()->appendRow({msgItem, msgDigestItem});
      }
   }

   {
      for (Structure::Output::Map::ConstIterator it = block.outputMap.constBegin(); it != block.outputMap.constEnd(); it++)
      {
         ModelItem* outputNumberItem = new ModelItem("#" + QString::number(it.key())); // read only
         ModelItem* outputItem = new ModelItem();

         tagRow(outputNumberItem, ":/PatchOutput.svg", Block::Marker::Output, it.key());
         invisibleRootItem()->appendRow({outputNumberItem, outputItem});
      }
   }

   endResetModel();
}
