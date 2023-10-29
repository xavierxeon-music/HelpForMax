#ifndef StructureH
#define StructureH

#include <QMap>
#include <QString>
#include <QStringList>

class Structure
{
public:
   enum class Type
   {
      Anything,
      Symbol,
      Float,
      Integer,
      Bang,
      List,
      Signal,
      MultiSignal,
      Dictionary,
      Matrix
   };

   enum PatcherType
   {
      PatcherStandard,
      PatcherGui,
      PatcherPoly,
      PatcherFourier
   };

   struct Base
   {
      bool undocumented = false;
   };

   struct Digest : Base
   {
      QString text;
      QString description;
   };

public:
   struct Patch
   {
      Digest digest;
      PatcherType patcherType = PatcherStandard;
      int inletCount = 0;
      QStringList metaTagList;
      QStringList seeAlsoList;
   };

   struct Output : Base
   {
      QString name;
      Digest digest;

      using Map = QMap<int, Output>; // outlet number vs port
   };

   // things in patcherargs without @
   // message arguments
   struct Argument : Base
   {
      QString name;
      bool optional = false;
      Type type = Type::Symbol;
      Digest digest;

      using List = QList<Argument>;
   };

   struct Message : Base
   {
      Argument::List arguments;
      Digest digest;

      using StandardMap = QMap<Type, Message>;
      using FreeMap = QMap<QString, Message>; // name vs message
   };

   // things in patcherargs with @
   struct Attribute : Base
   {
      bool get = true;
      bool set = true;
      Type type = Type::Symbol;
      int size = 1;
      Digest digest;

      using Map = QMap<QString, Attribute>; // name vs message
   };

   using SeeAlsoList = QStringList;

public:
   Patch patch;
   Output::Map outputMap;
   Argument::List argumentList;
   Attribute::Map attributeMap;
   Message::StandardMap messageStandardMap;
   Message::FreeMap messageUserDefinedMap;

public:
   virtual void clear();
   static QString typeName(const Type& type);
   static Type toType(const QString& name);
   static QList<Type> typeList();

private:
   static const QMap<Type, QString> typeNameMap;
};

#endif // NOT StructureH
