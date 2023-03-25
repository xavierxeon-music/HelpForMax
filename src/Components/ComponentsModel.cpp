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
   Q_UNUSED(key)

   if (enabled)
      update();
}

void ComponentsModel::rebuild()
{
   beginResetModel();

   clear();

   const PatchStructure& structure = mainWindow->parser();

   auto createUDocItem = [](const QString& iconPath, const PatchParser::Marker& marker, const QVariant& data)
   {
      ModelItem* udocItem = new ModelItem();
      udocItem->setIcon(QIcon(iconPath));

      udocItem->setData(QVariant::fromValue(marker), PatchParser::RoleMarker);
      udocItem->setData(data, PatchParser::RoleData);

      return udocItem;
   };

   {
      ModelItem* udocItem = createUDocItem(":/PatchGeneral.svg", PatchParser::Marker::Patch, true);

      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem();

      invisibleRootItem()->appendRow({udocItem, patchItem, patchDigestItem});
   }

   {
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchArgument.svg", PatchParser::Marker::Argument, index);

         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, argItem, argDigestItem});
      }
   }

   {
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchAttribute.svg", PatchParser::Marker::Attribute, it.key());

         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, attrItem, attrrDigestItem});
      }
   }

   {
      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* udocItem = createUDocItem(":/PatchMessage.svg", PatchParser::Marker::MessageStandard, QVariant::fromValue(type));

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
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchMessage.svg", PatchParser::Marker::MessageFree, it.key());

         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, msgItem, msgDigestItem});
      }
   }

   {
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         ModelItem* udocItem = createUDocItem(":/PatchOutput.svg", PatchParser::Marker::Output, it.key());

         ModelItem* outputNumberItem = new ModelItem("#" + QString::number(it.key())); // read only
         ModelItem* outputItem = new ModelItem();

         invisibleRootItem()->appendRow({udocItem, outputNumberItem, outputItem});
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

   {
      QStandardItem* udocItem = invisibleRootItem()->child(row, 0);
      setUdocStatus(udocItem, structure.patchDigest);

      QStandardItem* patchDigestItem = invisibleRootItem()->child(row, 2);
      patchDigestItem->setText(structure.patchDigest.text);
      row++;
   }

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
