#include "Block.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

#include "BlockHelp.h"
#include "BlockInitFile.h"
#include "BlockPatcher.h"
#include "BlockRef.h"

const QList<QByteArray> Block::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};

Block::Block(const QString& key, const QString& patchPath)
   : Structure()
   , key(key)
   , isUndocumented(false)
   , isModified(false)
   , patcher(nullptr)
   , ref(nullptr)
   , help(nullptr)
   , init(nullptr)
{
   patcher = new Patcher(this, patchPath);
   ref = new Ref(this);
   help = new Help(this);
   init = new InitFile(this);
}

Block::~Block()
{
   /*
   delete patcher;
   delete ref;
   delete help;
   delete init;
   */
}

Block::Map Block::compileMap(const QString& packagePath)
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
         if (fileInfo.fileName().startsWith("_"))
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

      Block* block = new Block(key, patchPath);
      block->load();

      map[key] = block;
   }

   QApplication::restoreOverrideCursor();

   return map;
}

void Block::clear()
{
   Structure::clear();
   isUndocumented = false;
}

bool Block::foundUndocumented() const
{
   return isUndocumented;
}

void Block::markModified()
{
   isModified = true;
}

const QString& Block::getPatchPath() const
{
   return patcher->getPath();
}

const QString& Block::getRefPath() const
{
   return ref->getPath();
}

QString Block::getRefContent()
{
   return ref->writeContent();
}

void Block::setRefContent(const QString& content)
{
   ref->readContent(content);
}

void Block::load()
{
   ref->read();
   patcher->read();

   if (patch.digest.text.isEmpty())
      patch.digest.text = "Hello World";

   if (!QFile::exists(ref->getPath()))
   {
      isModified = true;
      ref->write();
      help->write();
   }
}

void Block::save(Component component)
{
   if (Component::All == component)
   {
      if (!isModified)
         return;

      isModified = false;

      ref->write();
      help->write();
      init->write();

      clear();
      load();
   }
   else if (Component::InitFile == component)
   {
      init->write();
   }
}

void Block::markUndocumented(Base& base)
{
   base.undocumented = true;
   isUndocumented = true;
}
