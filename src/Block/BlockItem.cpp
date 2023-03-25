#include "BlockItem.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

const QList<QByteArray> Block::Item::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

Block::Item::Item(const QString& key)
   : Structure()
   , key(key)
   , isUndocumented(false)
   , patch(this)
   , ref(this)
   , help(this)
   , settings(this)
{
}

Block::Item::~Item()
{
}

Block::Item::Map Block::Item::compileMap(const QString& packageDir)
{
   qDebug() << packageDir;

   QDir().mkpath(packageDir + "/docs");
   QDir().mkpath(packageDir + "/help");
   QDir().mkpath(packageDir + "/init");
   QDir().mkpath(packageDir + "/patchers");

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

   recursiveSearch(packageDir + "/patchers");

   Map map;

   for (InfoMap::ConstIterator it = infoMap.constBegin(); it != infoMap.constEnd(); it++)
   {
      const QString key = it.key();
      const QString patchPath = it.value().absoluteFilePath();

      Item item(key);
      item.load(patchPath);

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
   return patch.getPatchPath();
}

const QString& Block::Item::getRefPath() const
{
   return QString();
}

void Block::Item::load(const QString& patchPath)
{
   ref.readXML();
   patch.addJSON(patchPath);

   if (patchDigest.text.isEmpty())
      patchDigest.text = "Hello World";

   /*
   if (!QFile::exists(refPath))
   {
      ref.writeXML();
      //writeHelpFile();
   }
*/
}

void Block::Item::markUndocumented(Base& base)
{
   base.undocumented = true;
   isUndocumented = true;
}
