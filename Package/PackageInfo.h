#ifndef PackageInfoH
#define PackageInfoH

#include <QFileInfo>
#include <QString>

#include "PatchInfo.h"

namespace Package
{
   class Info
   {
   public:
      const Patch::Info::Map& getPatchInfoMap() const;
      void updatePatchInfoMap();
      QStringList compileOrphanedFiles() const;

      Patch::Info findPatchInfo(const QString& patchFileName) const;
      const QString& getPath() const;
      const QString& getName() const;
      const QString& getAuthor() const;

   private:
      friend class Container;
      using Variable = QString Patch::Info::*;

   private:
      Info(const QString& path);

   private:
      void fillFiles(const QString& path, const bool scanSubFolders, const QString& extension, Variable entryVarialble);
      void fillFilesSources(const QString& path);

   private:
      const QString path;
      QString name;
      QString author;

      Patch::Info::Map patchInfoMap;
   };
} // namespace Package

#endif // NOT PackageInfoH
