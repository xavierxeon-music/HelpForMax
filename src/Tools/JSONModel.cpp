#include "JSONModel.h"

#include <limits.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

#include <Message.h>

QJsonObject JSON::fromFile(const QString& fileName)
{
   if (fileName.isEmpty())
      return QJsonObject();

   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
   {
      Message::error(QString("unalbe to read JSON file %1").arg(fileName));
      return QJsonObject();
   }

   const QByteArray content = file.readAll();
   file.close();

   QJsonParseError parserStatus;
   QJsonDocument doc = QJsonDocument::fromJson(content, &parserStatus);
   if (parserStatus.error != QJsonParseError::NoError)
   {
      Message::error(parserStatus.errorString());
      return QJsonObject();
   }

   QJsonObject object = doc.object();
   return object;
}

JSON::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
{
}

void JSON::Model::readFile(const QString& filePath)
{
   clear();

   setHorizontalHeaderLabels({"Name", "Type", "Value"});

   QJsonObject object = fromFile(filePath);
   if (object.empty())
      return;

   iterateObject(object, invisibleRootItem());
}

void JSON::Model::iterateObject(const QJsonObject& object, QStandardItem* parent)
{
   for (const QString& key : object.keys())
   {
      const QJsonValue value = object[key];
      addToModel(key, value, parent);
   }
}

void JSON::Model::iterateArray(const QJsonArray& array, QStandardItem* parent)
{
   const int itemCount = array.count();
   for (int index = 0; index < itemCount; index++)
   {
      const QString key = QString::number(index);
      const QJsonValue value = array.at(index);
      addToModel(key, value, parent);
   }
}

void JSON::Model::addToModel(const QString& key, const QJsonValue& value, QStandardItem* parent)
{
   ModelItem* nameItem = new ModelItem(key);
   ModelItem* typeItem = new ModelItem();
   ModelItem* valueItem = new ModelItem();

   switch (value.type())
   {
      case QJsonValue::Object:
      {
         typeItem->setText("object");

         const QJsonObject childObject = value.toObject();
         iterateObject(childObject, nameItem);

         valueItem->setText(QString("%1 keys").arg(childObject.keys().count()));
         break;
      }
      case QJsonValue::Array:
      {
         typeItem->setText("array");

         const QJsonArray childArray = value.toArray();
         iterateArray(childArray, nameItem);

         valueItem->setText(QString("%1 entries").arg(childArray.count()));
         break;
      }
      case QJsonValue::Bool:
      {
         typeItem->setText("boolean");

         const bool boolean = value.toBool();
         valueItem->setText(boolean ? "true" : "fasle");
         break;
      }
      case QJsonValue::Double:
      {
         static const qint64 defaultNumber = std::numeric_limits<qint64>::max();

         const qint64 intNumber = value.toInteger(defaultNumber);
         if (defaultNumber != intNumber)
         {
            typeItem->setText("integer");
            valueItem->setText(QString::number(intNumber));
         }
         else
         {
            typeItem->setText("float");
            const double floatNumber = value.toDouble();
            valueItem->setText(QString::number(floatNumber));
         }

         break;
      }
      case QJsonValue::String:
      {
         typeItem->setText("string");

         const QString& string = value.toString();
         valueItem->setText(string);
         break;
      }

      default:
      {
         typeItem->setText("value");
         break;
      }
   }

   parent->appendRow({nameItem, typeItem, valueItem});
}
