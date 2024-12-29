#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::Argument)
{
}

void Suggest::Model::Argument::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      bool active = true;
      if (structure.argumentList.count() > row)
      {
         const Ref::Structure::Argument& argumentStructure = structure.argumentList.at(row);
         const Ref::Structure::Argument& argumentSuggest = suggest.argumentList.at(row);
         active = (argumentSuggest.dataType != argumentStructure.dataType);
      }

      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 1);

      typeItem->setData(active, DataActive);

      const QBrush& fgBRush = active ? brushActive : brushInactive;
      typeItem->setForeground(fgBRush);
      digestItem->setForeground(fgBRush);
   }
   emit signalDataEdited();
}

void Suggest::Model::Argument::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Digest"});

   removeContent();

   for (int row = 0; row < suggest.argumentList.count(); row++)
   {
      const Ref::Structure::Argument& argument = suggest.argumentList.at(row);

      QStandardItem* typeItem = new QStandardItem();
      typeItem->setEditable(false);
      typeItem->setText(Max::dataTypeName(argument.dataType));
      typeItem->setData(QVariant::fromValue(argument.dataType), DataMarker);

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);
      digestItem->setText(argument.digest.shortText);

      invisibleRootItem()->appendRow({typeItem, digestItem});
   }

   endResetModel();
   update();
}

void Suggest::Model::Argument::buildStructure()
{
   const Max::Object::List patcherArgs = suggest.findAll(Max::Object::Type::PatcherArgs, true);
   const Max::Object::List unpackArgs = suggest.findAll(Max::Object::Type::Unpack, true);

   for (const Max::Object* object : patcherArgs)
   {
      const Max::Object* unpackObject = nullptr;
      for (const Max::Object* object2 : unpackArgs)
      {
         if (object2->isChildOf(object))
         {
            unpackObject = object2;
            break;
         }
      }

      const int argumentIndex = object->text.indexOf("@");
      const QString text = object->text.mid(0, argumentIndex);
      const QStringList argumentNameList = text.split(" ", Qt::SkipEmptyParts);

      const QStringList unpackTypeList = unpackObject ? unpackObject->text.split(" ", Qt::SkipEmptyParts) : QStringList();

      for (int index = 1; index < argumentNameList.count(); index++)
      {
         Ref::Structure::Argument argument;
         argument.digest.shortText = " (" + argumentNameList.at(index) + ")";

         if (unpackTypeList.size() > index)
         {
            const QString type = unpackTypeList.at(index);
            const Max::DataType dataType = Max::tagDataType(type.at(0));
            if (Max::DataType::Undefined != dataType)
               argument.dataType = dataType;
         }

         suggest.argumentList.append(argument);
      }
   }
}

void Suggest::Model::Argument::transfer(const QList<int>& rowList)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      if (!rowList.contains(row))
         continue;

      QStandardItem* typeItem = invisibleRootItem()->child(row, 0);
      const bool active = typeItem->data(DataActive).toBool();
      if (!active)
         continue;

      if (structure.argumentList.count() <= row)
      {
         Ref::Structure::Argument argument;
         structure.argumentList.append(argument);
      }

      Ref::Structure::Argument& argumentStructure = structure.argumentList[row];
      const Ref::Structure::Argument& argumentSuggest = suggest.argumentList.at(row);

      argumentStructure.dataType = argumentSuggest.dataType;
      argumentStructure.digest.shortText = argumentSuggest.digest.shortText;
   }
}
