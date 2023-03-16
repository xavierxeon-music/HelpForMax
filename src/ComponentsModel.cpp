#include "ComponentsModel.h"

#include <ModelItem.h>

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

void ComponentsModel::setModified(bool enabled)
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
      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem();

      invisibleRootItem()->appendRow({patchItem, patchDigestItem});
      addMarker(PatchParser::Marker::Patch, true, patchItem, patchDigestItem);

      patchItem->setIcon(QIcon(":/PatchGeneral.svg"));
   }

   {
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({argItem, argDigestItem});
         addMarker(PatchParser::Marker::Argument, index, argItem, argDigestItem);

         argItem->setIcon(QIcon(":/PatchArgument.svg"));
      }
   }

   {
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({attrItem, attrrDigestItem});
         addMarker(PatchParser::Marker::Attribute, it.key(), attrItem, attrrDigestItem);

         attrItem->setIcon(QIcon(":/PatchAttribute.svg"));
      }
   }

   {
      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* msgItem = new ModelItem(PatchStructure::typeName(type)); // read only
         ModelItem* msgDigestItem = new ModelItem();

         if (structure.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(structure.messageStandardMap.value(type).digest.text);
         }
         else
         {
            msgItem->setVisible(false);
         }

         invisibleRootItem()->appendRow({msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageStandard, QVariant::fromValue(type), msgItem, msgDigestItem);

         msgItem->setIcon(QIcon(":/PatchMessage.svg"));
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         invisibleRootItem()->appendRow({msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageFree, it.key(), msgItem, msgDigestItem);

         msgItem->setIcon(QIcon(":/PatchMessage.svg"));
      }
   }

   {
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         ModelItem* outputItem = new ModelItem();
         ModelItem* outputNumberItem = new ModelItem("output #" + QString::number(it.key())); // read only

         invisibleRootItem()->appendRow({outputItem, outputNumberItem});
         addMarker(PatchParser::Marker::Output, it.key(), outputItem, outputNumberItem);

         outputItem->setIcon(QIcon(":/PatchOutput.svg"));
      }
   }

   endResetModel();
}

void ComponentsModel::update()
{
   const PatchStructure structure = mainWindow->parser();
   int row = 0;

   QStandardItem* patchDigestItem = invisibleRootItem()->child(row, 1);
   patchDigestItem->setText(structure.patchDigest.text);
   row++;

   {
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         const PatchStructure::Argument& argument = structure.argumentList.at(index);

         QStandardItem* argItem = invisibleRootItem()->child(row + index, 0);
         argItem->setText(argument.name);

         QStandardItem* argDigestItem = invisibleRootItem()->child(row + index, 1);
         argDigestItem->setText(argument.digest.text);
      }
      row += structure.argumentList.count();
   }

   {
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         QStandardItem* attrrDigestItem = invisibleRootItem()->child(row, 1);
         attrrDigestItem->setText(it.value().digest.text);
         row++;
      }
   }

   {
      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* msgItem = static_cast<ModelItem*>(invisibleRootItem()->child(row, 0));
         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 1);

         if (structure.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(structure.messageStandardMap.value(type).digest.text);
            msgItem->setVisible(true);
         }
         else
         {
            msgItem->setVisible(false);
         }

         row++;
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         QStandardItem* msgDigestItem = invisibleRootItem()->child(row, 1);
         msgDigestItem->setText(it.value().digest.text);

         row++;
      }
   }

   {
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         QStandardItem* outputItem = invisibleRootItem()->child(row, 0);
         outputItem->setText(it.value().name);

         row++;
      }
   }
}
