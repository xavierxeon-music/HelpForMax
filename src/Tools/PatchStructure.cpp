#include "PatchStructure.h"

const QMap<PatchStructure::Type, QString> PatchStructure::typeNameMap = {{Type::Unkown, "???"}, // must be max names!
                                                                         {Type::Bang, "bang"},
                                                                         {Type::Integer, "int"},
                                                                         {Type::Float, "float"},
                                                                         {Type::Symbol, "symbol"},
                                                                         {Type::List, "list"},
                                                                         {Type::Signal, "signal"}};

void PatchStructure::clear()
{
   *this = PatchStructure();
}

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
