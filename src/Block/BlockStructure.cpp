#include "BlockStructure.h"

const QMap<Block::Structure::Type, QString> Block::Structure::typeNameMap = {{Type::Unkown, "???"}, // must be max names!
                                                                             {Type::Bang, "bang"},
                                                                             {Type::Integer, "int"},
                                                                             {Type::Float, "float"},
                                                                             {Type::Symbol, "symbol"},
                                                                             {Type::List, "list"},
                                                                             {Type::Signal, "signal"},
                                                                             {Type::MultiSignal, "multichannelsignal"}};

void Block::Structure::clear()
{
   *this = Structure();
}

QString Block::Structure::typeName(const Type& type)
{
   return typeNameMap.value(type, "???");
}

Block::Structure::Type Block::Structure::toType(const QString& name)
{
   for (QMap<Type, QString>::ConstIterator it = typeNameMap.constBegin(); it != typeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return Type::Unkown;
}

QList<Block::Structure::Type> Block::Structure::typeList()
{
   return typeNameMap.keys();
}
