#include "PatchInfo.h"

#include <QDir>
#include <QFileInfo>

bool Patch::Info::isOrphan() const
{
   return (patchPath.isEmpty() && sourcePath.isEmpty());
}

QString Patch::Info::getName() const
{
   QFileInfo patchFile(patchPath);
   return patchFile.fileName().replace(".maxpat", "");
}

QString Patch::Info::getFolder() const
{
   QFileInfo patchFile(patchPath);
   return patchFile.dir().dirName();
}
