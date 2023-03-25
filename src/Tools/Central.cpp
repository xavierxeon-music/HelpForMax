#include "Central.h"

#include "Tools/Settings.h"

// function hub

void FunctionHub::setPackagePath(QString packageDir)
{
   // do nothing
   Q_UNUSED(packageDir);
}

void FunctionHub::setModified(bool enabled, QString key)
{
   // do nothing
   Q_UNUSED(enabled)
   Q_UNUSED(key)
}

void FunctionHub::patchSelected(QString patchPath, QString key)
{
   // do nothing
   Q_UNUSED(patchPath)
   Q_UNUSED(key)
}

void FunctionHub::componentSelected(Block::Item::Marker marker, QVariant data)
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
   , blockMap()
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

void Central::compileBlockMap(const QString& packagePath)
{
   blockMap = Block::Item::compileMap(packagePath);
}

const Block::Item::Map Central::getBlockMap() const
{
   return blockMap;
}

const Block::Item& Central::block() // must not be constant, else map iterator is not not a reference
{
   return blockMap[currentKey];
}

Block::Item& Central::blockRef()
{
   return blockMap[currentKey];
}

void Central::selectBlock(const QString& key)
{
   currentKey = key;
}

bool Central::isBlockUndocumented(const QString& key) const
{
   if (!blockMap.contains(key))
      return false;

   return blockMap[key].foundUndocumented();
}

/*
void Central::loadPatchStructure(QString patchPath, const QString& key)
{
   parserMap[key] = PatchParser(patchPath);
}

void Central::selectPatchStructure(const QString& key)
{
   currentKey = key;
}

bool Central::isPatchStructureUndocumented(const QString& key) const
{
   if (!parserMap.contains(key))
      return false;

   return parserMap[key].foundUndocumented();
}

QString Central::getRefPath(const QString& key) const
{
   static const QString dummy;
   if (!parserMap.contains(key))
      return dummy;

   const QString refPath = parserMap[key].getRefPath();
   return refPath;
}

void Central::savePatchStructures()
{
   for (PatchParser& parser : parserMap)
   {
      parser.writeXML();
      parser.clear();
      parser.load();

      parser.writeHelpFile();
      parser.writeInitFile();
   }

   callOnAllHubInstances(&FunctionHub::setModified, false, QString());
}
*/
