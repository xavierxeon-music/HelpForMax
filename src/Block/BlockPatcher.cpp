#include "BlockPatcher.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonValue>

#include "Block.h"
#include "Tools/JSONModel.h"

// max class

Block::Patcher::MaxClass::MaxClass(const QJsonObject& boxObject)
   : boxType()
   , arguments()
   , id()
{
   id = boxObject["id"].toString();

   QString text = boxObject["text"].toString();

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

   boxType = contentList.first();
   arguments = contentList.mid(1);
}

// patcher

Block::Patcher::Patcher(Block* block, const QString& patchPath)
   : block(block)
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
   MessageMap messageMap = compileInletMessageMap(patcherObject, {"route", "routepass", "typeroute~"});

   for (const MaxClass& maxClass : messageMap["route"])
      addRouteMessage(maxClass);
   for (const MaxClass& maxClass : messageMap["routepass"])
      addRouteMessage(maxClass);
   for (const MaxClass& maxClass : messageMap["typeroute~"])
      addTypeRouteMessage(maxClass, patcherObject);
}

Block::Patcher::Inlet::ConnectionMap Block::Patcher::compileInletConnectionMap(const QJsonObject patcherObject)
{
   Inlet::ConnectionMap inletConnectionMap;

   // parse inlets, outlets and patcherargs
   const QJsonArray boxArray = patcherObject["boxes"].toArray();
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
         const QString comment = boxObject["comment"].toString();
         Structure::Output& output = findOrCreateOutput(index, comment);

         if (output.name.isEmpty())
         {
            block->markUndocumented(output);
            output.name = comment;
         }
      }
      else if ("newobj" == className)
      {
         MaxClass maxClass(boxObject);

         if ("patcherargs" == maxClass.boxType)
            readPatcherargs(maxClass.arguments);
         else if ("wa.setup.bpatcher" == maxClass.boxType)
            block->patch.patcherType = Structure::PatcherGui;
         else if ("wa.setup.poly" == maxClass.boxType)
            block->patch.patcherType = Structure::PatcherPoly;
         else if ("wa.setup.pfft" == maxClass.boxType)
            block->patch.patcherType = Structure::PatcherFourier;
      }
   }

   // find objects connected to inlets
   const QJsonArray lineArray = patcherObject["lines"].toArray();
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

   return inletConnectionMap;
}

Block::Patcher::MessageMap Block::Patcher::compileInletMessageMap(const QJsonObject patcherObject, const QStringList& boxTypeList)
{
   Inlet::ConnectionMap inletConnectionMap = compileInletConnectionMap(patcherObject);
   block->patch.inletCount = inletConnectionMap.count();

   auto connectedToInlet = [&](const QJsonObject& boxObject) -> bool
   {
      const QString routeId = boxObject["id"].toString();
      for (Inlet::ConnectionMap::ConstIterator it = inletConnectionMap.constBegin(); it != inletConnectionMap.constEnd(); it++)
      {
         for (const QString& id : it.value().connectedIdList)
         {
            if (id == routeId)
               return true;
         }
      }

      return false;
   };

   MessageMap messageMap;
   for (const QString& boxType : boxTypeList)
      messageMap[boxType] = MaxClass::List();

   // parse routes connected to inlets for  messages
   const QJsonArray boxArray = patcherObject["boxes"].toArray();
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
         const QStringList contentList = text.split(" ", Qt::SkipEmptyParts);

         // box type we are interested in
         const QString className = contentList.first();
         if (!messageMap.contains(className))
            continue;

         // check if connected to inlet
         if (!connectedToInlet(boxObject))
            continue;

         MaxClass maxClass(boxObject);
         messageMap[className].append(maxClass);
      }
   }

   return messageMap;
}

void Block::Patcher::addRouteMessage(const MaxClass& maxClass)
{
   for (const QString& messageText : maxClass.arguments)
   {
      const Structure::Type type = Structure::toType(messageText);
      if (Structure::Type::Anything == type)
      {
         if (!block->messageUserDefinedMap.contains(messageText))
         {
            block->messageUserDefinedMap[messageText] = Structure::Message();
            block->markUndocumented(block->messageUserDefinedMap[messageText]);
         }

         Structure::Message& message = block->messageUserDefinedMap[messageText];
         if (message.arguments.empty())
         {
            message.arguments.append(Structure::Argument());
         }
      }
      else
      {
         if (!block->messageStandardMap.contains(type))
         {
            block->messageStandardMap[type] = Structure::Message();
            block->markUndocumented(block->messageStandardMap[type]);
         }

         Structure::Message& message = block->messageStandardMap[type];
         if (message.arguments.empty())
         {
            message.arguments.append(Structure::Argument());
         }
      }
   }
}

void Block::Patcher::addTypeRouteMessage(const MaxClass& maxClass, const QJsonObject patcherObject)
{
   static const QList<Structure::Type> portTypeList = {Structure::Type::Signal,
                                                       Structure::Type::Bang,
                                                       Structure::Type::Integer,
                                                       Structure::Type::Float,
                                                       Structure::Type::Symbol,
                                                       Structure::Type::List};

   const QJsonArray lineArray = patcherObject["lines"].toArray();
   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      if (maxClass.id != sourceId)
         continue;

      int port = sourceArray.at(1).toInt();
      const Structure::Type type = portTypeList.at(port);

      if (!block->messageStandardMap.contains(type))
      {
         block->messageStandardMap[type] = Structure::Message();
         block->markUndocumented(block->messageStandardMap[type]);
      }

      Structure::Message& message = block->messageStandardMap[type];
      if (message.arguments.empty())
      {
         message.arguments.append(Structure::Argument());
      }
   }
}

Structure::Output& Block::Patcher::findOrCreateOutput(const int id, const QString& name)
{
   Output::Map& blockMap = block->outputMap;

   // first pass: by name
   for (Output::Map::iterator it = blockMap.begin(); it != blockMap.end(); it++)
   {
      Structure::Output& output = it.value();
      if (output.name == name)
         return output;
   }

   // second pass by matching input
   for (Output::Map::iterator it = blockMap.begin(); it != blockMap.end(); it++)
   {
      if (id == it.key())
         return it.value();
   }

   // create new
   Structure::Output output{};
   block->markUndocumented(output);

   blockMap[id] = output;
   return blockMap[id];
}

void Block::Patcher::readPatcherargs(const QStringList& arguments)
{
   enum class State
   {
      Argument,
      AttributeName,
      AttributeArg
   };

   State state = State::Argument;
   for (int i = 0; i < arguments.size(); i++)
   {
      const QString& arg = arguments.at(i);

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

         if (i > block->argumentList.count())
         {
            block->markUndocumented(argument);
            block->argumentList.append(argument);
         }
      }
      else if (State::AttributeName == state)
      {
         Structure::Attribute attribute;
         const QString& name = arg.mid(1);
         if (!block->attributeMap.contains(name))
         {
            block->markUndocumented(attribute);
            block->attributeMap[name] = attribute;
         }
      }
      else if (State::AttributeArg == state)
      {
      }
   }
}
