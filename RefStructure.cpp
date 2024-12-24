#include "RefStructure.h"

// structure

const QList<QByteArray> Ref::Structure::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

using PatchTypeNameMap = QMap<Ref::Structure::PatchType, QString>;
static const PatchTypeNameMap patchTypeNameMap = {{Ref::Structure::PatchType::Standard, "standard"},
                                                  {Ref::Structure::PatchType::Gui, "gui"},
                                                  {Ref::Structure::PatchType::Poly, "poly"},
                                                  {Ref::Structure::PatchType::Fourier, "fourier"}};

using PatchPartNameMap = QMap<Ref::Structure::PatchPart, QString>;
static const PatchPartNameMap patchPartNameMap = {{Ref::Structure::PatchPart::Undefined, "undefined"},
                                                  {Ref::Structure::PatchPart::Header, "Header"},
                                                  {Ref::Structure::PatchPart::Argument, "Argument"},
                                                  {Ref::Structure::PatchPart::Attribute, "Attribute"},
                                                  {Ref::Structure::PatchPart::MessageTyped, "Message Typed"},
                                                  {Ref::Structure::PatchPart::MessageNamed, "Message Named"},
                                                  {Ref::Structure::PatchPart::Output, "Output"}};

Ref::Structure::Structure()
   : header()
   , outputMap()
   , argumentList()
   , messageTypedMap()
   , messageNamedMap()
   , clearHook()
   , dirtyHook()
{
   for (const Max::DataType& dataType : Max::dataTypeList())
   {
      {
         Structure::MessageTyped message;
         message.active = false;
         message.dataType = dataType;

         messageTypedMap[dataType] = message;
      }
      {
         Structure::Output output;
         output.active = false;
         output.dataType = dataType;

         outputMap[dataType] = output;
      }
   }
}

Ref::Structure::~Structure()
{
}

void Ref::Structure::clear()
{
   *this = Structure();

   if (clearHook)
      clearHook();
}

void Ref::Structure::setDirty()
{
   if (dirtyHook)
      dirtyHook();
}

QString Ref::Structure::patchTypeName(const PatchType& type)
{
   return patchTypeNameMap.value(type, "standard");
}

Ref::Structure::PatchType Ref::Structure::toPatchType(const QString& name)
{
   for (PatchTypeNameMap::const_iterator it = patchTypeNameMap.constBegin(); it != patchTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchType::Standard;
   ;
}

QList<Ref::Structure::PatchType> Ref::Structure::patchTypeList()
{
   return patchTypeNameMap.keys();
}

QString Ref::Structure::partName(const PatchPart& part)
{
   return patchPartNameMap.value(part, "undefined");
}

QIcon Ref::Structure::partIcon(const PatchPart& part)
{
   if (PatchPart::Header == part)
      return QIcon(":/PatchGeneral.svg");
   else if (PatchPart::Argument == part)
      return QIcon(":/DocArgument.svg");
   else if (PatchPart::Attribute == part)
      return QIcon(":/DocAttribute.svg");
   else if (PatchPart::MessageTyped == part)
      return QIcon(":/DocMessageTyped.svg");
   else if (PatchPart::MessageNamed == part)
      return QIcon(":/DocMessageNamed.svg");
   else if (PatchPart::Output == part)
      return QIcon(":/DocOutput.svg");
   else if (PatchPart::Other == part)
      return QIcon(":/OverviewGeneral.svg");
   return QIcon();
}

Ref::Structure::PatchPart Ref::Structure::toPart(const QString& name)
{
   for (PatchPartNameMap ::const_iterator it = patchPartNameMap.constBegin(); it != patchPartNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return PatchPart::Undefined;
   ;
}

void Ref::Structure::repackNamedMessages()
{
   AttributesAndMessageNamed::List list = messageNamedMap.values();
   messageNamedMap.clear();

   for (const AttributesAndMessageNamed& message : list)
   {
      QString name = message.name;
      // avoid duplicate keys
      while (messageNamedMap.contains(name))
         name += "_";

      messageNamedMap.insert(name, message);
   }
}
