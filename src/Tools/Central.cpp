#include "Central.h"

#include <QApplication>
#include <QPalette>

#include "Tools/JSONModel.h"
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

void FunctionHub::componentSelected(Block::Marker marker, QVariant data)
{
   // do nothing
   Q_UNUSED(marker)
   Q_UNUSED(data)
}

void FunctionHub::restoreState()
{
}

void FunctionHub::saveState()
{
}

// central

const QBrush Central::udocBrush = QApplication::palette().alternateBase();
const QBrush Central::docBrush = QApplication::palette().base();

QString Central::packageAuthor = QString();
QString Central::packageName = QString();

Central::Central()
   : currentKey()
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
   blockMap = Block::compileMap(packagePath);
}

const Block::Map Central::getBlockMap() const
{
   return blockMap;
}

const Block& Central::block() // must not be constant, else map iterator is not not a reference
{
   return *blockMap[currentKey];
}

Block& Central::blockRef()
{
   return *blockMap[currentKey];
}

void Central::selectBlock(const QString& key)
{
   currentKey = key;
}

bool Central::isBlockUndocumented(const QString& key) const
{
   if (!blockMap.contains(key))
      return false;

   return blockMap[key]->foundUndocumented();
}

void Central::saveBlocks(Block::Component component)
{
   for (Block* block : std::as_const(blockMap))
   {
      block->save(component);
   }
}

void Central::readPackageInfo(QString packagePath)
{
   const QString fileName = packagePath + "/package-info.json";

   QJsonObject object = JSON::fromFile(fileName);
   if (object.empty())
   {
      packageAuthor = "";
      packageName = "";
      return;
   }

   packageAuthor = object["author"].toString();
   packageName = object["name"].toString();

   // qDebug() << packageAuthor << packageName;
}
