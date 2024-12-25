#include "SuggestModelNamedMessage.h"

#include "DiscreteMathsAlgorithm.h"

Suggest::Model::NamedMessage::NamedMessage(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::MessageNamed)
{
}

void Suggest::Model::NamedMessage::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      QStandardItem* typeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* isAttributeItem = invisibleRootItem()->child(row, 2);
      QStandardItem* isMessageItem = invisibleRootItem()->child(row, 3);

      if (!nameItem)
         continue;

      const QString name = nameItem->data(DataMarker).toString();

      //const Ref::Structure::AttributesAndMessageNamed& messageNamedStructure = structure.messageNamedMap.value(name);
      //const Ref::Structure::AttributesAndMessageNamed& messageNamedSuggest = suggest.messageNamedMap.value(name);
   }

   emit signalDataEdited();
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
      nameItem->setData(it.key(), DataMarker);

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
   update();
}

void Suggest::Model::NamedMessage::buildStructure()
{
   // attributes
   auto valueType = [&](const QString& value) -> Max::DataType
   {
      if (value.startsWith("@"))
         return Max::DataType::Undefined;

      bool isFloat = false;
      for (const QChar& c : value)
      {
         if ("." == c)
         {
            isFloat = true;
            continue;
         }
         if (c.isLetter())
            return Max::DataType::Symbol;
      }

      if (isFloat)
         return Max::DataType::Float;

      return Max::DataType::Integer;
   };

   const Max::Object::List patcherArgs = suggest.findAll(Max::Object::Type::PatcherArgs, true);
   for (const Max::Object* object : patcherArgs)
   {
      const int attributeIndex = object->text.indexOf("@");
      const QString text = object->text.mid(attributeIndex, -1);
      const QStringList attributeNameList = text.split(" ", Qt::SkipEmptyParts);

      for (int index = 0; index < attributeNameList.count(); index++)
      {
         QString key = attributeNameList.at(index);
         if (!key.contains("@"))
            continue;

         key = key.mid(1, -1);
         index++;
         if (index >= attributeNameList.count())
            break;

         QString value = attributeNameList.at(index);
         const Max::DataType dataType = valueType(value);
         if (Max::DataType::Undefined == dataType)
            continue;

         Ref::Structure::AttributesAndMessageNamed attribute;
         attribute.patchParts = Ref::Structure::PatchPart::Attribute;
         attribute.name = key;
         attribute.dataType = dataType;
         suggest.messageNamedMap.insert(key, attribute);
      }
   }

   // messages
   DiscreteMaths::Algorithm algo(&suggest);

   const Max::Object::List inlets = suggest.findAll(Max::Object::Type::Inlet, false);
   auto connectedToInlet = [&](const Max::Object* object) -> bool
   {
      const int objectIndex = suggest.vertexIndex(object);
      for (const Max::Object* inlet : inlets)
      {
         DiscreteMaths::Algorithm::Tree tree = algo.breadthFirst(inlet);
         const int depth = tree.compileDepth(objectIndex);

         if (0 < depth)
            return true;
      }

      return false;
   };

   const Max::Object::List routeArgs = suggest.findAll(Max::Object::Type::Route, true) + suggest.findAll(Max::Object::Type::RoutePass, true);
   for (const Max::Object* object : routeArgs)
   {
      if (!connectedToInlet(object)) // this is an attribute only
         continue;

      const QStringList messageList = object->text.split(" ", Qt::SkipEmptyParts);

      for (int index = 1; index < messageList.count(); index++)
      {
         const QString key = messageList.at(index);
         const Max::DataType dataType = Max::toDataType(key);
         if (Max::DataType::Undefined != dataType)
            continue;

         if (!suggest.messageNamedMap.contains(key))
         {
            Ref::Structure::AttributesAndMessageNamed message;
            message.name = key;
            suggest.messageNamedMap.insert(key, message);
         }

         Ref::Structure::AttributesAndMessageNamed& message = suggest.messageNamedMap[key];
         message.patchParts |= Ref::Structure::PatchPart::MessageNamed;
      }
   }
}

void Suggest::Model::NamedMessage::transfer(const QList<int>& rowList)
{
   qDebug() << __FUNCTION__ << rowList;
}
