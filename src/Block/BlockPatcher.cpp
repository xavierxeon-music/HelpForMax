#include "BlockPatcher.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonValue>

#include "BlockItem.h"
#include "Tools/JSONModel.h"

Block::Patcher::Patcher(Item* item, const QString& patchPath)
   : item(item)
   , patchPath(patchPath)
{
}

const QString& Block::Patcher::getPath() const
{
   return patchPath;
}

void Block::Patcher::read()
{
   QJsonObject object = JSON::fromFile(patchPath);
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();
   const QJsonArray boxArray = patcherObject["boxes"].toArray();
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   struct Inlet
   {
      int index;
      QStringList connectedIdList;
   };

   using InletConnectionMap = QMap<QString, Inlet>;
   InletConnectionMap inletConnectionMap;

   // parse inlets, outlets and patcherargs
   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if ("inlet" == className)
      {
         const int index = boxObject["index"].toInt();
         const QString id = boxObject["id"].toString();
         inletConnectionMap[id] = {index, QStringList()};
      }
      else if ("outlet" == className)
      {
         const int index = boxObject["index"].toInt();
         Structure::Output& output = findOrCreateOutput(index);

         const QString comment = boxObject["comment"].toString();
         if (output.name.isEmpty())
         {
            item->markUndocumented(output);
            output.name = comment;
         }
      }
      else if ("newobj" == className)
      {
         QString text = boxObject["text"].toString();
         //

         if (text.startsWith("patcherargs"))
            readPatcherargs(text);
         else if (text.startsWith("wa.setup.bpatcher"))
            item->patch.openAsBPatcher = true;
      }
   }

   item->patch.inletCount = inletConnectionMap.count();

   // find objects connected to inlets
   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();

      if (inletConnectionMap.contains(destId))
         inletConnectionMap[destId].connectedIdList.append(sourceId);
      else if (inletConnectionMap.contains(sourceId))
         inletConnectionMap[sourceId].connectedIdList.append(destId);
   }

   // parse routes connected to inlets for  messages
   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if ("newobj" == className)
      {
         const QString text = boxObject["text"].toString();
         if (!text.startsWith("route"))
            continue;

         // check if connected to inlet
         int connectedIndex = [&]() -> int
         {
            const QString routeId = boxObject["id"].toString();
            for (InletConnectionMap::ConstIterator it = inletConnectionMap.constBegin(); it != inletConnectionMap.constEnd(); it++)
            {
               for (const QString& id : it.value().connectedIdList)
               {
                  if (id == routeId)
                     return it.value().index;
               }
            }
            return -1;
         }();

         if (-1 == connectedIndex)
            continue;

         const QStringList contentList = text.split(" ", Qt::SkipEmptyParts);
         for (int i = 1; i < contentList.size(); i++)
         {
            const QString& messageText = contentList.at(i);
            const Structure::Type type = Structure::toType(messageText);
            if (Structure::Type::Unkown == type)
            {
               if (!item->messageFreeMap.contains(messageText))
               {
                  item->messageFreeMap[messageText] = Structure::Message();
                  item->markUndocumented(item->messageFreeMap[messageText]);
               }

               Structure::Message& message = item->messageFreeMap[messageText];
               if (message.arguments.empty())
               {
                  message.arguments.append(Structure::Argument());
               }
            }
            else
            {
               if (!item->messageStandardMap.contains(type))
               {
                  item->messageStandardMap[type] = Structure::Message();
                  item->markUndocumented(item->messageStandardMap[type]);
               }

               Structure::Message& message = item->messageStandardMap[type];
               if (message.arguments.empty())
               {
                  message.arguments.append(Structure::Argument());
               }
            }
         }
      }
   }
}

Block::Structure::Output& Block::Patcher::findOrCreateOutput(const int id)
{
   if (!item->outputMap.contains(id))
   {
      item->outputMap[id] = Structure::Output{};
      item->markUndocumented(item->outputMap[id]);
   }

   return item->outputMap[id];
}

void Block::Patcher::readPatcherargs(QString text)
{
   // remove whitespace in quotes
   bool inQuote = false;
   for (int index = 0; index < text.size(); index++)
   {
      if ("\"" == text.mid(index, 1))
      {
         inQuote ^= true;
         continue;
      }
      else if (inQuote && " " == text.mid(index, 1))
      {
         text.replace(index, 1, "_");
      }
   }

   const QStringList contentList = text.split(" ", Qt::SkipEmptyParts);
   enum class State
   {
      Argument,
      AttributeName,
      AttributeArg
   };

   State state = State::Argument;

   for (int i = 1; i < contentList.size(); i++)
   {
      const QString& arg = contentList.at(i);

      // maybe advance state
      if (arg.startsWith("@"))
         state = State::AttributeName;
      else if (State::AttributeName == state)
         state = State::AttributeArg;

      if (State::Argument == state)
      {
         Structure::Argument argument;
         argument.name = QString::number(i);
         argument.optional = true;

         if (i > item->argumentList.count())
         {
            item->markUndocumented(argument);
            item->argumentList.append(argument);
         }
      }
      else if (State::AttributeName == state)
      {
         Structure::Attribute attribute;
         const QString& name = arg.mid(1);
         if (!item->attributeMap.contains(name))
         {
            item->markUndocumented(attribute);
            item->attributeMap[name] = attribute;
         }
      }
      else if (State::AttributeArg == state)
      {
      }
   }
}
