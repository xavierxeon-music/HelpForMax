#include "HelpComponentsModel.h"

#include <ModelItem.h>

Help::ComponentsModel::ComponentsModel(Persona* persona)
   : QStandardItemModel(persona)
   , Persona::FunctionHub()
   , persona(persona)
{
}

void Help::ComponentsModel::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   rebuild();
}

void Help::ComponentsModel::rebuild()
{
   beginResetModel();

   clear();

   const PatchStructure structure = persona->parser();

   auto addMarker = [](const PatchParser::Marker& marker, const QVariant& data, ModelItem* item1, ModelItem* item2)
   {
      item1->setData(QVariant::fromValue(marker), PatchParser::RoleMarker);
      item1->setData(data, PatchParser::RoleData);

      item2->setData(QVariant::fromValue(marker), PatchParser::RoleMarker);
      item2->setData(data, PatchParser::RoleData);
   };

   {
      ModelItem* patchItem = new ModelItem("PATCH");
      ModelItem* patchDigestItem = new ModelItem(structure.patchDigest.text);

      invisibleRootItem()->appendRow({patchItem, patchDigestItem});
      addMarker(PatchParser::Marker::Patch, true, patchItem, patchDigestItem);
   }

   {
      ModelItem* argumentListItem = new ModelItem("ARGUMENTS");
      invisibleRootItem()->appendRow(argumentListItem);
      argumentListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (int index = 0; index < structure.argumentList.count(); index++)
      {
         const PatchStructure::Argument& argument = structure.argumentList.at(index);

         ModelItem* argItem = new ModelItem(argument.name);
         ModelItem* argDigestItem = new ModelItem(argument.digest.text);

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
         ModelItem* attrItem = new ModelItem(it.key());
         ModelItem* attrrDigestItem = new ModelItem(it.value().digest.text);

         attributeListItem->appendRow({attrItem, attrrDigestItem});
         addMarker(PatchParser::Marker::Attribute, it.key(), attrItem, attrrDigestItem);
      }
   }

   {
      ModelItem* messageListItem = new ModelItem("MESSAGES");
      invisibleRootItem()->appendRow(messageListItem);
      messageListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (PatchStructure::Message::FreeMap::ConstIterator it = structure.messageFreeMap.constBegin(); it != structure.messageFreeMap.constEnd(); it++)
      {
         ModelItem* msgItem = new ModelItem(it.key());
         ModelItem* msgDigestItem = new ModelItem(it.value().digest.text);

         messageListItem->appendRow({msgItem, msgDigestItem});
         addMarker(PatchParser::Marker::Message, it.key(), msgItem, msgDigestItem);
      }
   }

   {
      ModelItem* outputListItem = new ModelItem("OUTPUTS");
      invisibleRootItem()->appendRow(outputListItem);
      outputListItem->setData(QVariant::fromValue(PatchParser::Marker::Undefined), PatchParser::RoleMarker);

      for (PatchStructure::Output::Map::ConstIterator it = structure.outputMap.constBegin(); it != structure.outputMap.constEnd(); it++)
      {
         ModelItem* outputItem = new ModelItem(it.value().name);
         ModelItem* outputDigestItem = new ModelItem(QString::number(it.key()));

         outputListItem->appendRow({outputItem, outputDigestItem});
         addMarker(PatchParser::Marker::Output, it.key(), outputItem, outputDigestItem);
      }
   }

   endResetModel();
}
