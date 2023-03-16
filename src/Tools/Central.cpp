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

QString Central::packageAuthor = QString();
QString Central::packageName = QString();

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
   return packageAuthor;
}

QString Central::getPackageName()
{
   return packageName;
}

const QString& Central::getCurrentKey() const
{
   return currentKey;
}

const PatchParser& Central::parser() // must not be constant, else map iterator is not not a reference
{
   return parserMap[currentKey];
}

PatchParser& Central::parserRef()
{
   return parserMap[currentKey];
}

void Central::loadPatchStructure(QString patchPath, const QString& key)
{
   parserMap[key] = PatchParser(patchPath);
}

void Central::selectPatchStructure(const QString& key)
{
   currentKey = key;
}

bool Central::isPatchStructureUndocumented(const QString& key)
{
   if (!parserMap.contains(key))
      return false;

   return parserMap[key].foundUndocumented();
}

void Central::savePatchStructures()
{
   for (PatchParser& parser : parserMap)
      parser.writeXML();

   callOnOtherHubInstances(&FunctionHub::setModified, false);
}
