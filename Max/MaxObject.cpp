#include "MaxObject.h"

#include <QJsonArray>

Max::Object::Object(const QJsonObject& boxObject)
   : DiscreteMaths::Vertex()
   , patchRect()
   , className()
   , text()
   , comment()
   , inlets(0)
   , outlets(0)
   , type(Type::Other)
   , id()
   , isParamObject(false)
{
   className = boxObject["maxclass"].toString();

   QJsonArray patchRectData = boxObject["patching_rect"].toArray();
   patchRect.setX(patchRectData[0].toDouble());
   patchRect.setY(patchRectData[1].toDouble());
   patchRect.setWidth(patchRectData[2].toDouble());
   patchRect.setHeight(patchRectData[3].toDouble());

   text = boxObject["text"].toString().simplified();
   if ("inlet" == className)
   {
      const int index = boxObject["index"].toInt();
      text = "IN\n" + QString::number(index);
      comment = boxObject["comment"].toString();
      type = Type::Inlet;
   }
   else if ("outlet" == className)
   {
      const int index = boxObject["index"].toInt();
      text = "OUT\n" + QString::number(index);
      comment = boxObject["comment"].toString();
      type = Type::Outlet;
   }
   else if (text.isEmpty())
   {
      text = className;
      // qDebug() << "NO TEXT" << className;
   }
   else if ("newobj" == className)
   {
      using TypeMap = QMap<QString, Type>;
      static const TypeMap typeMap = {{"patcherargs", Type::PatcherArgs},
                                      {"route", Type::Route},
                                      {"routepass", Type::RoutePass},
                                      {"typeroute~", Type::TypeRoute},
                                      {"unpack", Type::Unpack}};

      const QString typeString = text.split(" ", Qt::SkipEmptyParts).first();
      type = typeMap.value(typeString, Type::Other);
   }

   using NameMap = QMap<Type, QString>;
   static const NameMap nameMap = {{Type::Inlet, "Inlet"},
                                   {Type::Outlet, "Outlet"},
                                   {Type::PatcherArgs, "PatcherArgs"},
                                   {Type::Route, "Route"},
                                   {Type::RoutePass, "RoutePass"},
                                   {Type::TypeRoute, "TypeRoute"},
                                   {Type::Unpack, "Unpack"}};

   id = boxObject["id"].toString();
   name = nameMap.value(type, "Other") + " (" + id + ")";
   if (!comment.isEmpty())
      name += " " + comment;

   inlets.count = boxObject["numinlets"].toInt();
   outlets.count = boxObject["numoutlets"].toInt();
}

bool Max::Object::isChildOf(const Object* parent) const
{
   return inlets.objects.contains(parent);
}
