#include "PackageInfo.h"

#include <QDir>

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

Package::Info::Info(const QString& path)
   : path(path)
   , name()
   , author()
{
}
