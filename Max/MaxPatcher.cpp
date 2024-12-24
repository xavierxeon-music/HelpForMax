#include "MaxPatcher.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "DiscreteMathsAlgorithm.h"

Max::Patcher::Patcher()
   : Ref::Structure()
   , DiscreteMaths::Graph()
   , typeBuffer()
{
}

void Max::Patcher::readPatch(const QString& patchFileName)
{
   typeBuffer.clear();
   DiscreteMaths::Graph::clear(true);

   if (patchFileName.isEmpty())
      return;

   QFile file(patchFileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   qDebug() << patchFileName;

   const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   file.close();

   QJsonObject object = doc.object();
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();

   const IdMap idMap = readObjects(patcherObject);
   readLines(patcherObject, idMap);

   analyse();

   Ref::Structure::clear();
   buildStructureArguments();
   buildStructureTypedMessages();
   buildStructureNamedMessages();
   buildStructureOutputs();
}

Max::Object* Max::Patcher::getVertexCast(int vertexIndex) const
{
   Object* vertex = static_cast<Object*>(getVertex(vertexIndex));
   return vertex;
}

Max::Line* Max::Patcher::getEdgeCast(int edgeIndex) const
{
   Line* edge = static_cast<Line*>(getEdge(edgeIndex));
   return edge;
}

void Max::Patcher::analyse()
{
   static const QList<Object::Type> sourceTypeList = {Object::Type::PatcherArgs, Object::Type::Inlet};
   static const QList<Object::Type> processTypeList = {Object::Type::Route, Object::Type::RoutePass, Object::Type::TypeRoute, Object::Type::Unpack};

   const Object::List sources = findAll(sourceTypeList, false);
   const Object::List processors = findAll(processTypeList, false);

   DiscreteMaths::Algorithm algo(this);
   for (Object* source : sources)
   {
      const DiscreteMaths::Algorithm::Tree tree = algo.breadthFirst(source);
      for (Object* processor : processors)
      {
         const int targetIndex = vertexIndex(processor);
         const DiscreteMaths::Algorithm::Path path = tree.compilePath(targetIndex);
         const int depth = path.verticies.count();

         auto pathIsValid = [&]()
         {
            if (0 == depth)
               return false;

            for (int index = 0; index < depth; index++)
            {
               const int vertexIndexA = path.verticies.at(index);
               Object* outletObject = getVertexCast(vertexIndexA);

               if (!sourceTypeList.contains(outletObject->type) && !processTypeList.contains(outletObject->type))
                  return false;
            }
            return true;
         };

         if (!pathIsValid())
            continue;

         for (int index = 1; index < depth; index++)
         {
            const int vertexIndexA = path.verticies.at(index);
            Object* outletObject = getVertexCast(vertexIndexA);
            outletObject->isParamObject = true;

            const int vertexIndexB = path.verticies.at(index - 1);
            Object* inletObject = getVertexCast(vertexIndexB);
            inletObject->isParamObject = true;

            const int edgeIndex = findEdgeIndex(outletObject, inletObject);
            Line* line = getEdgeCast(edgeIndex);
            line->isParamLine = true;
         }
      }
   }
}

void Max::Patcher::buildStructureArguments()
{
   const Object::List patcherArgs = findAll(Object::Type::PatcherArgs, true);
   const Object::List unpackArgs = findAll(Object::Type::Unpack, true);

   for (const Object* object : patcherArgs)
   {
      const Object* unpackObject = nullptr;
      for (const Object* object2 : unpackArgs)
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

         argumentList.append(argument);
      }
   }
}

void Max::Patcher::buildStructureTypedMessages()
{
   const Object::List routeArgs = findAll(Object::Type::Route, true) + findAll(Object::Type::RoutePass, true);
   for (const Object* object : routeArgs)
   {
      const QStringList argumentNameList = object->text.split(" ", Qt::SkipEmptyParts);
      for (int index = 1; index < argumentNameList.count(); index++)
      {
         const QString type = argumentNameList.at(index);
         const Max::DataType dataType = Max::toDataType(type);
         if (Max::DataType::Undefined == dataType)
            continue;

         messageTypedMap[dataType].active = true;
      }
   }

   const Object::List typeRouteArgs = findAll(Object::Type::TypeRoute, true);
   for (const Object* object : typeRouteArgs)
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
         messageTypedMap[dataType].active = true;
      }
   }
}

void Max::Patcher::buildStructureNamedMessages()
{
   // attributes
   const Object::List patcherArgs = findAll(Object::Type::PatcherArgs, true);
   for (const Object* object : patcherArgs)
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
         if (value.contains("@"))
            continue;

         Ref::Structure::AttributesAndMessageNamed attribute;
         attribute.patchParts = Ref::Structure::PatchPart::Attribute;
         attribute.name = key;
         messageNamedMap.insert(key, attribute);
      }
   }

   DiscreteMaths::Algorithm algo(this);

   const Object::List inlets = findAll(Object::Type::Inlet, false);
   auto connectedToInlet = [&](const Object* object) -> bool
   {
      const int objectIndex = vertexIndex(object);
      for (const Object* inlet : inlets)
      {
         DiscreteMaths::Algorithm::Tree tree = algo.breadthFirst(inlet);
         const int depth = tree.compileDepth(objectIndex);

         if (0 < depth)
            return true;
      }

      return false;
   };

   // messages
   const Object::List routeArgs = findAll(Object::Type::Route, true) + findAll(Object::Type::RoutePass, true);
   for (const Object* object : routeArgs)
   {
      if (!connectedToInlet(object)) // this is an attribute only
         continue;

      const QStringList messageList = object->text.split(" ", Qt::SkipEmptyParts);

      for (int index = 1; index < messageList.count(); index++)
      {
         const QString key = messageList.at(index);
         if (!messageNamedMap.contains(key))
         {
            Ref::Structure::AttributesAndMessageNamed message;
            message.name = key;
            messageNamedMap.insert(key, message);
         }

         Ref::Structure::AttributesAndMessageNamed& message = messageNamedMap[key];
         message.patchParts |= Ref::Structure::PatchPart::MessageNamed;
      }
   }
}

void Max::Patcher::buildStructureOutputs()
{
}

Max::Object::List Max::Patcher::findAll(const Object::Type& type, bool paramObjectsOnly) const
{
   return findAll(QList<Object::Type>() << type, paramObjectsOnly);
}

Max::Object::List Max::Patcher::findAll(const QList<Object::Type>& typeList, bool paramObjectsOnly) const
{
   Object::List list;
   for (const Object::Type& type : typeList)
   {
      if (!typeBuffer.contains(type))
         continue;

      if (paramObjectsOnly)
      {
         for (Object* object : typeBuffer.value(type))
         {
            if (object->isParamObject)
               list.append(object);
         }
      }
      else
      {
         list.append(typeBuffer.value(type));
      }
   }

   return list;
}

Max::IdMap Max::Patcher::readObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};

   IdMap idMap;
   const QJsonArray boxArray = patcherObject["boxes"].toArray();

   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if (skipList.contains(className))
         continue;

      Object* object = new Object(boxObject);
      addVertex(object);

      Object::List& typeList = typeBuffer[object->type];
      if (!typeList.contains(object))
         typeList.append(object);

      idMap[object->id] = object;
   }

   return idMap;
}

void Max::Patcher::readLines(const QJsonObject patcherObject, const IdMap& idMap)
{
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      Line* line = new Line(lineObject, idMap);
      addEdge(line);
   }
}
