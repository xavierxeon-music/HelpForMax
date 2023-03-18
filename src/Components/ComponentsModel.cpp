#include "ComponentsModel.h"

#include "Tools/ModelItem.h"

#include "MainWindow.h"

ComponentsModel::ComponentsModel(MainWindow* mainWindow)
   : QStandardItemModel(mainWindow)
   , FunctionHub()
   , mainWindow(mainWindow)
{
}

void ComponentsModel::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   rebuild();
   update();
}

void ComponentsModel::setModified(bool enabled, QString key)
{
   if (enabled)
      update();
}

void ComponentsModel::rebuild()
{
   beginResetModel();

   clear();

   const PatchStructure& structure = mainWindow->parser();

   auto addMarker = [](const PatchParser::Marker& marker, const QVariant& data, ModelItem* item1, ModelItem* item2)
   {
      item1->setData(QVariant::fromValue(marker), PatchParser::RoleMarker);
      item1->setData(data, PatchParser::RoleData);

      item2->setData(QVariant::fromValue(marker), PatchParser::RoleMarker);
      item2->setData(data, PatchParser::RoleData);
   };

   {
      ModelItem* udocItem = new ModelItem();
      udocItem->setIcon(QIcon(":/PatchGeneral.svg"));

      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem();

      invisibleRootItem()->appendRow({udocItem, patchItem, patchDigestItem});
      addMarker(PatchParser::Marker::Patch, true, patchItem, patchDigestItem);

   }

   {
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         ModelItem* udocItem = new ModelItem();
         udocItem->setIcon(QIcon(":/PatchArgument.svg"));

         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, argItem, argDigestItem});
         addMarker(PatchParser::Marker::Argument, index, argItem, argDigestItem);
      }
   }

   {
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         ModelItem* udocItem = new ModelItem();
         udocItem->setIcon(QIcon(":/PatchAttribute.svg"));

         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, attrItem, attrrDigestItem});
         addMarker(PatchParser::Marker::Attribute, it.key(), attrItem, attrrDigestItem);
      }
   }

   {
      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* udocItem = new ModelItem();
         udocItem->setIcon(QIcon(":/PatchMessage.svg"));

         ModelItem* msgItem = new ModelItem(PatchStructure::typeName(type)); // read only
         ModelItem* msgDigestItem = new ModelItem();

         if (structure.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(structure.messageStandardMap.value(type).digest.text);
         }
         else
         {
            udocItem->setVisible(false);
         }

         invisibleRootItem()->appendRow({udocItem, msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageStandard, QVariant::fromValue(type), msgItem, msgDigestItem);
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         ModelItem* udocItem = new ModelItem();
         udocItem->setIcon(QIcon(":/PatchMessage.svg"));

         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageFree, it.key(), msgItem, msgDigestItem);
      }
   }

   {
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         ModelItem* udocItem = new ModelItem();
         udocItem->setIcon(QIcon(":/PatchOutput.svg"));

         ModelItem* outputNumberItem = new ModelItem("#" + QString::number(it.key())); // read only
         ModelItem* outputItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, outputNumberItem, outputItem});
         addMarker(PatchParser::Marker::Output, it.key(), outputItem, outputNumberItem);
      }
   }

   endResetModel();
}

void ComponentsModel::update()
{
   const PatchStructure structure = mainWindow->parser();
   int row = 0;

   auto setUdocStatus = [&](QStandardItem* item, const PatchStructure::Base& base, bool append = false)
   {
      if (base.undocumented)
         item->setText("*");
      else if (!append)
         item->setText("");
   };

   QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
   setUdocStatus(udocItem, structure.patchDigest);

   QStandardItem* patchDigestItem = invisibleRootItem()->child(row, 2);
   patchDigestItem->setText(structure.patchDigest.text);
   row++;

   {
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         const PatchStructure::Argument& argument = structure.argumentList.at(index);

         QStandardItem* udocItem = invisibleRootItem()->child(row + index, 0);
         setUdocStatus(udocItem, argument);
         setUdocStatus(udocItem, argument.digest, true);

         QStandardItem* argItem = invisibleRootItem()->child(row + index, 1);
         argItem->setText(argument.name);

         QStandardItem* argDigestItem = invisibleRootItem()->child(row + index, 2);
         argDigestItem->setText(argument.digest.text);
      }
      row += structure.argumentList.count();
   }

   {
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         const PatchStructure::Attribute& attribute = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, attribute);
         setUdocStatus(udocItem, attribute.digest, true);

         QStandardItem* attrrDigestItem = invisibleRootItem()->child(row, 2);
         attrrDigestItem->setText(attribute.digest.text);
         row++;
      }
   }

   {
      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* udocItem = static_cast<ModelItem*>(invisibleRootItem()->child(row, 0));
         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 2);

         if (structure.messageStandardMap.contains(type))
         {
            const PatchStructure::Message& message = structure.messageStandardMap.value(type);

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

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         const PatchStructure::Message& message = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, message);

         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 2);
         msgDigestItem->setText(message.digest.text);

         row++;
      }
   }

   {
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         const PatchStructure::Output& output = it.value();

         QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
         setUdocStatus(udocItem, output);
         setUdocStatus(udocItem, output.digest);

         QStandardItem* outputItem = invisibleRootItem()->child(row, 2);
         outputItem->setText(output.name);

         row++;
      }
   }
}
