#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::Argument)
{
}

void Suggest::Model::Argument::update()
{
}

void Suggest::Model::Argument::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Name", "Type"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (int row = 0; row < suggest.argumentList.count(); row++)
   {
      const Ref::Structure::Argument& argument = suggest.argumentList.at(row);

      QStandardItem* nameItem = new QStandardItem();
      nameItem->setEditable(false);
      nameItem->setText(argument.name);

      qDebug() << argument.name;

      QStandardItem* typeItem = new QStandardItem();
      nameItem->setEditable(false);
      typeItem->setText(Max::dataTypeName(argument.dataType));

      invisibleRootItem()->appendRow({nameItem, typeItem});
   }

   endResetModel();
   update();

   emit signalDataEdited();
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
         argument.name = argumentNameList.at(index);

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
   qDebug() << __FUNCTION__ << rowList;
}
