#include "PackageInfo.h"

#include <QDir>

// entry

bool Package::Info::Entry::isOrphan() const
{
   return (patchPath.isEmpty() && sourcePath.isEmpty());
}

// info

Package::Info::Info(const QString& path)
   : path(path)
   , name()
   , author()
   , entryMap()
{
   updateEntries();
}

const Package::Info::Entry::Map& Package::Info::getEntryMap() const
{
   return entryMap;
}

void Package::Info::updateEntries()
{
   entryMap.clear();

   fillEntryMap(path + "/patchers", true, ".maxpat", &Entry::patchPath);
   fillEntryMap(path + "/help", true, ".maxhelp", &Entry::refPath);
   fillEntryMap(path + "/docs", true, ".maxref.xml", &Entry::helpPath);
   fillEntryMap(path + "/init", false, ".txt", &Entry::initPath);
   fillEntrySources(path + "/source/maxobjects");
   fillEntrySources(path + "/source/maxglobal");
}

QStringList Package::Info::compileOrphanFiles() const
{
   QStringList fileList;
   for (Entry::Map::const_iterator it = entryMap.constBegin(); it != entryMap.constEnd(); ++it)
   {
      const Entry& entry = it.value();
      if (!entry.isOrphan())
         continue;

      if (!entry.refPath.isEmpty())
         fileList.append(entry.refPath);

      if (!entry.helpPath.isEmpty())
         fileList.append(entry.helpPath);

      if (!entry.initPath.isEmpty())
         fileList.append(entry.initPath);
   }

   return fileList;
}

Patch::Info Package::Info::extractPatchInfo(const QString& patchFileName) const
{
   QFileInfo patchFile(patchFileName);
   Patch::Info patchInfo;
   patchInfo.name = patchFile.fileName().replace(".maxpat", "");
   patchInfo.folder = patchFile.dir().dirName();

   return patchInfo;
}

const QString& Package::Info::getPath() const
{
   return path;
}

const QString& Package::Info::getName() const
{
   return name;
}

const QString& Package::Info::getAuthor() const
{
   return author;
}

void Package::Info::fillEntryMap(const QString& path, const bool scanSubFolders, const QString& extension, Entry::Variable entryVarialble)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      if (contentInfo.isDir() && scanSubFolders)
      {
         fillEntryMap(contentInfo.absoluteFilePath(), false, extension, entryVarialble);
         continue;
      }
      if (!contentInfo.isFile() || !contentInfo.fileName().endsWith(extension))
         continue;

      const QString name = contentInfo.fileName().replace(extension, "");
      Entry& entry = entryMap[name];
      entry.*entryVarialble = contentInfo.absoluteFilePath();
   }
}

void Package::Info::fillEntrySources(const QString& path)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      const QString name = contentInfo.fileName().replace("_tilde", "~");
      Entry& entry = entryMap[name];
      entry.sourcePath = contentInfo.absoluteFilePath();
   }
}
