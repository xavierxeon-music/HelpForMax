#include "Central.h"

#include <Settings.h>

// function hub

void FunctionHub::setPackagePath(QString packageDir)
{
   // do nothing
   Q_UNUSED(packageDir);
}

void FunctionHub::setModified(bool enabled)
{
   // do nothing
   Q_UNUSED(enabled);
}

void FunctionHub::patchSelected(QString patchPath, QString key)
{
   // do nothing
   Q_UNUSED(patchPath)
   Q_UNUSED(key)
}

void FunctionHub::componentSelected(PatchParser::Marker marker, QVariant data)
{
   // do nothing
   Q_UNUSED(marker)
   Q_UNUSED(data)
}

// central

Central::Central()
   : FunctionHub()
   , currentKey()
   , parserMap()
{
}

QString Central::getPackagePath()
{
   Settings settings;
   const QString packagePath = settings.string("LastPackage");

   return packagePath;
}

QString Central::getAuthor()
{
   return "Ralf Waspe";
}

QString Central::getPackageName()
{
   return "WaMaxPackage";
}

const QString& Central::getCurrentKey() const
{
   return currentKey;
}

PatchParser Central::parser() const
{
   return parserMap[currentKey];
}

PatchParser& Central::parserRef()
{
   return parserMap[currentKey];
}

void Central::buildPatchStructure(QString patchPath, const QString& key)
{
   currentKey = key;

   if (!parserMap.contains(key))
      parserMap[key] = PatchParser(patchPath);
}

void Central::savePatchStructures()
{
   for (PatchParser& parser : parserMap)
      parser.writeXML();

   callOnOtherHubInstances(&FunctionHub::setModified, false);
}
