#include "PackageInfo.h"

#include <QDir>

Package::Info::Info(const QString& path)
   : path(path)
   , name()
   , author()
   , patchInfoMap()
{
   updatePatchInfoMap();
}

const Patch::Info::Map& Package::Info::getPatchInfoMap() const
{
   return patchInfoMap;
}

void Package::Info::updatePatchInfoMap()
{
   patchInfoMap.clear();

   fillFiles(path + "/patchers", true, ".maxpat", &Patch::Info::patchPath);
   fillFiles(path + "/help", true, ".maxhelp", &Patch::Info::refPath);
   fillFiles(path + "/docs", true, ".maxref.xml", &Patch::Info::helpPath);
   fillFiles(path + "/init", false, ".txt", &Patch::Info::initPath);
   fillFilesSources(path + "/source/maxobjects");
   fillFilesSources(path + "/source/maxglobal");
}

QStringList Package::Info::compileOrphanedFiles() const
{
   QStringList fileList;
   for (Patch::Info::Map::const_iterator it = patchInfoMap.constBegin(); it != patchInfoMap.constEnd(); ++it)
   {
      const Patch::Info& entry = it.value();
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

Patch::Info Package::Info::findPatchInfo(const QString& patchFileName) const
{
   static const Patch::Info dummy;

   QFileInfo patchFile(patchFileName);
   const QString key = patchFile.fileName().replace(".maxpat", "");

   return patchInfoMap.value(key, dummy);
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

void Package::Info::fillFiles(const QString& path, const bool scanSubFolders, const QString& extension, Variable entryVarialble)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      if (contentInfo.isDir() && scanSubFolders)
      {
         fillFiles(contentInfo.absoluteFilePath(), false, extension, entryVarialble);
         continue;
      }
      if (!contentInfo.isFile() || !contentInfo.fileName().endsWith(extension))
         continue;

      const QString name = contentInfo.fileName().replace(extension, "");
      Patch::Info& entry = patchInfoMap[name];
      entry.*entryVarialble = contentInfo.absoluteFilePath();
   }
}

void Package::Info::fillFilesSources(const QString& path)
{
   QDir dir(path);
   const QFileInfoList content = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
   for (const QFileInfo& contentInfo : content)
   {
      const QString name = contentInfo.fileName().replace("_tilde", "~");
      Patch::Info& entry = patchInfoMap[name];
      entry.sourcePath = contentInfo.absoluteFilePath();
   }
}
