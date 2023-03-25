#include "BlockItem.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

const QList<QByteArray> Block::Item::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

/*
Block::Item::Item()
   : Item(QString(), QString())
{
}
*/

Block::Item::Item(const QString& key, const QString& patchPath)
   : Structure()
   , key(key)
   , isUndocumented(false)
   , patch(this, patchPath)
   , ref(this)
   , help(this)
   , settings(this)
{
}

/*
Block::Item::Item(const Item& other)
   : Item(QString(), QString())
{
   *this = other;
}
*/

Block::Item::~Item()
{
}

/*
Block::Item& Block::Item::operator=(const Item& other)
{
   key = other.key;
   isUndocumented = other.isUndocumented;

   return *this;
}
*/

Block::Item::Map Block::Item::compileMap(const QString& packagePath)
{
   QDir().mkpath(packagePath + "/docs");
   QDir().mkpath(packagePath + "/help");
   QDir().mkpath(packagePath + "/init");
   QDir().mkpath(packagePath + "/patchers");

   using InfoMap = QMap<QString, QFileInfo>;

   InfoMap infoMap;
   const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs;
   std::function<void(const QString&)> recursiveSearch = [&](const QString& path)
   {
      const QFileInfoList infoList = QDir(path).entryInfoList(filters);
      for (const QFileInfo& fileInfo : infoList)
      {
         if (fileInfo.isDir())
         {
            recursiveSearch(fileInfo.absoluteFilePath());
            continue;
         }

         if (!fileInfo.fileName().endsWith(".maxpat"))
            continue;

         const QString key = fileInfo.fileName().replace(".maxpat", "");
         infoMap[key] = fileInfo;
      }
   };

   QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

   recursiveSearch(packagePath + "/patchers");

   Map map;

   for (InfoMap::ConstIterator it = infoMap.constBegin(); it != infoMap.constEnd(); it++)
   {
      const QString key = it.key();
      const QString patchPath = it.value().absoluteFilePath();

      Item* item = new Item(key, patchPath);
      item->load();

      map[key] = item;
   }

   QApplication::restoreOverrideCursor();

   return map;
}

void Block::Item::clear()
{
   Structure::clear();
   isUndocumented = false;
}

bool Block::Item::foundUndocumented() const
{
   return isUndocumented;
}

const QString& Block::Item::getPatchPath() const
{
   return patch.getPath();
}

const QString& Block::Item::getRefPath() const
{
   return ref.getPath();
}

void Block::Item::load()
{
   ref.read();
   patch.read();

   if (patchDigest.text.isEmpty())
      patchDigest.text = "Hello World";

   if (!QFile::exists(ref.getPath()))
   {
      ref.write();
      help.write();
   }
}

void Block::Item::save()
{
   ref.write();
   help.write();
   settings.write();

   clear();
   load();
}

void Block::Item::markUndocumented(Base& base)
{
   base.undocumented = true;
   isUndocumented = true;
}
