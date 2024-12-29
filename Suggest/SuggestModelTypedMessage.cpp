#include "SuggestModelTypedMessage.h"

Suggest::Model::TypedMessage::TypedMessage(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::MessageTyped)
{
}

void Suggest::Model::TypedMessage::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      QStandardItem* activeItem = invisibleRootItem()->child(row, 1);

      const Max::DataType type = typeItem->data(DataMarker).value<Max::DataType>();
      const Ref::Structure::MessageTyped& messageStructure = structure.messageTypedMap[type];
      const Ref::Structure::MessageTyped& messageSuggest = suggest.messageTypedMap[type];

      const bool active = (messageSuggest.active != messageStructure.active);
      typeItem->setData(active, DataActive);

      const QBrush& fgBRush = active ? brushActive : brushInactive;
      typeItem->setForeground(fgBRush);
      activeItem->setForeground(fgBRush);
   }

   emit signalDataEdited();
}

void Suggest::Model::TypedMessage::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Active"});

   removeContent();

   for (const Max::DataType& type : Max::dataTypeList())
   {
      const Ref::Structure::MessageTyped& message = suggest.messageTypedMap[type];

      QStandardItem* typeItem = new QStandardItem(Max::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type), DataMarker);

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(false);
      activeItem->setCheckState(message.active ? Qt::Checked : Qt::Unchecked);
      activeItem->setIcon(QIcon(":/DocMessageTyped.svg"));

      invisibleRootItem()->appendRow({typeItem, activeItem});
   }

   endResetModel();
   update();
}

void Suggest::Model::TypedMessage::buildStructure()
{
   const Max::Object::List routeArgs = suggest.findAll({Max::Object::Type::Route, Max::Object::Type::RoutePass}, true);
   for (const Max::Object* object : routeArgs)
   {
      const QStringList argumentNameList = object->text.split(" ", Qt::SkipEmptyParts);
      for (int index = 1; index < argumentNameList.count(); index++)
      {
         const QString type = argumentNameList.at(index);
         const Max::DataType dataType = Max::toDataType(type);
         if (Max::DataType::Undefined == dataType)
            continue;

         suggest.messageTypedMap[dataType].active = true;
      }
   }

   const Max::Object::List typeRouteArgs = suggest.findAll(Max::Object::Type::TypeRoute, true);
   for (const Max::Object* object : typeRouteArgs)
   {
      static const QList<Max::DataType> typeList = {
         Max::DataType::Signal,
         Max::DataType::Bang,
         Max::DataType::Integer,
         Max::DataType::Float,
         Max::DataType::Symbol
         //, Max::DataType::List
      };

      for (const int& index : object->outlets.connected)
      {
         if (index >= typeList.count())
            continue;

         const Max::DataType dataType = typeList.at(index);
         suggest.messageTypedMap[dataType].active = true;
      }
   }
}

void Suggest::Model::TypedMessage::transfer(const QList<int>& rowList)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      if (!rowList.contains(row))
         continue;

      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      const bool active = typeItem->data(DataActive).toBool();
      if (!active)
         continue;

      const Max::DataType type = typeItem->data(DataMarker).value<Max::DataType>();
      qDebug() << type << structure.messageTypedMap.contains(type);

      Ref::Structure::MessageTyped& messageStructure = structure.messageTypedMap[type];
      const Ref::Structure::MessageTyped& messageSuggest = suggest.messageTypedMap[type];

      messageStructure.active = messageSuggest.active;
   }
}
