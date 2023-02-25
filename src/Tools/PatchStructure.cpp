#include "PatchStructure.h"

QString PatchStructure::typeName(const Type& type)
{
   switch (type)
   {
      case Type::Symbol:
         return "symbol";
      case Type::Float:
         return "float";
      case Type::Integer:
         return "int";
      case Type::Bang:
         return "bang";
      case Type::List:
         return "list";
      case Type::Signal:
         return "signal";
      default:
         break;
   }

   return QString("???");
}

PatchStructure::Type PatchStructure::toType(const QString& name)
{
   if ("symbol" == name)
      return Type::Symbol;
   else if ("float" == name)
      return Type::Float;
   else if ("int" == name)
      return Type::Integer;
   else if ("bang" == name)
      return Type::Bang;
   else if ("list" == name)
      return Type::List;
   else if ("signal" == name)
      return Type::Signal;

   return Type::Unkown;
}
