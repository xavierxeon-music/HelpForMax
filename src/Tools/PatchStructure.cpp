#include "PatchStructure.h"

const QMap<PatchStructure::Type, QString> PatchStructure::typeNameMap = {{Type::Unkown, "???"}, // must be max names!
                                                                         {Type::Symbol, "symbol"},
                                                                         {Type::Float, "float"},
                                                                         {Type::Integer, "int"},
                                                                         {Type::Bang, "bang"},
                                                                         {Type::List, "list"},
                                                                         {Type::Signal, "signal"}};

QString PatchStructure::typeName(const Type& type)
{
   return typeNameMap.value(type, "???");
}

PatchStructure::Type PatchStructure::toType(const QString& name)
{
   for (QMap<Type, QString>::ConstIterator it = typeNameMap.constBegin(); it != typeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return Type::Unkown;
}

QList<PatchStructure::Type> PatchStructure::typeList()
{
   return typeNameMap.keys();
}
