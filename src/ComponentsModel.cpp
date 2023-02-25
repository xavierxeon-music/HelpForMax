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

   const PatchStructure structure = mainWindow->parser();

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
   }

   {
      ModelItem* argumentListItem = new ModelItem("ARGUMENTS");
      invisibleRootItem()->appendRow(argumentListItem);
      argumentListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         ModelItem* argItem = new ModelItem();
         ModelItem* argDigestItem = new ModelItem();

         argumentListItem->appendRow({argItem, argDigestItem});
         addMarker(PatchParser::Marker::Argument, index, argItem, argDigestItem);
      }
   }

   {
      ModelItem* attributeListItem = new ModelItem("ATTRIBUTES");
      invisibleRootItem()->appendRow(attributeListItem);
      attributeListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         ModelItem* attrItem = new ModelItem(it.key()); // read only
         ModelItem* attrrDigestItem = new ModelItem();

         attributeListItem->appendRow({attrItem, attrrDigestItem});
         addMarker(PatchParser::Marker::Attribute, it.key(), attrItem, attrrDigestItem);
      }
   }

   {
      ModelItem* messageListItem = new ModelItem("MESSAGES");
      invisibleRootItem()->appendRow(messageListItem);
      messageListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

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

         messageListItem->appendRow({msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageStandard, QVariant::fromValue(type), msgItem, msgDigestItem);
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         ModelItem* msgItem = new ModelItem(it.key()); // read only
         ModelItem* msgDigestItem = new ModelItem();

         messageListItem->appendRow({msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::MessageFree, it.key(), msgItem, msgDigestItem);
      }
   }

   {
      ModelItem* outputListItem = new ModelItem("OUTPUTS");
      invisibleRootItem()->appendRow(outputListItem);
      outputListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         ModelItem* outputItem = new ModelItem();
         ModelItem* outputNumberItem = new ModelItem("output #" + QString::number(it.key())); // read only

         outputListItem->appendRow({outputItem, outputNumberItem});
         addMarker(PatchParser::Marker::Output, it.key(), outputItem, outputNumberItem);
      }
   }

   endResetModel();
}

void ComponentsModel::update()
{
   const PatchStructure structure = mainWindow->parser();

   QStandardItem* patchDigestItem = invisibleRootItem()->child(0, 1);
   patchDigestItem->setText(structure.patchDigest.text);

   {
      QStandardItem* argumentListItem = invisibleRootItem()->child(1, 0);
      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         const PatchStructure::Argument& argument = structure.argumentList.at(index);

         QStandardItem* argItem = argumentListItem->child(index, 0);
         argItem->setText(argument.name);

         QStandardItem* argDigestItem = argumentListItem->child(index, 1);
         argDigestItem->setText(argument.digest.text);
      }
   }

   {
      QStandardItem* attributeListItem = invisibleRootItem()->child(2, 0);
      int index = 0;
      for (PatchStructure::Attribute::Map::ConstIterator it = structure.attributeMap.constBegin(); it != structure.attributeMap.constEnd(); it++)
      {
         QStandardItem* attrrDigestItem = attributeListItem->child(index, 1);
         attrrDigestItem->setText(it.value().digest.text);
         index++;
      }
   }

   {
      QStandardItem* messageListItem = invisibleRootItem()->child(3, 0);
      int index = 0;

      for (const PatchStructure::Type& type : PatchStructure::typeList())
      {
         ModelItem* msgItem = static_cast<ModelItem*>(messageListItem->child(index, 0));
         QStandardItem* msgDigestItem = messageListItem->child(index, 1);

         if (structure.messageStandardMap.contains(type))
         {
            msgDigestItem->setText(structure.messageStandardMap.value(type).digest.text);
            msgItem->setVisible(true);
         }
         else
         {
            msgItem->setVisible(false);
         }

         index++;
      }

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         QStandardItem* msgDigestItem = messageListItem->child(index, 1);
         msgDigestItem->setText(it.value().digest.text);

         index++;
      }
   }

   {
      QStandardItem* outputListItem = invisibleRootItem()->child(4, 0);

      int index = 0;
      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         QStandardItem* outputItem = outputListItem->child(index, 0);
         outputItem->setText(it.value().name);

         index++;
      }
   }
}
